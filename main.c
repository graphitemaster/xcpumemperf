#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <float.h>
#include <limits.h>
#include <assert.h>
#include <pthread.h>

#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/socket.h>

/* How many times to run the benchmark (Default) */
#define TRIALS 128

/* The benchmark memory to operate on (Default) */
#define MEMORY (size_t)(32ull * 1024ull * 1024ull) // 32MiB

/* Simple binary semaphore with pthread mutex and condition variable */
struct bsem {
	pthread_mutex_t m;
	pthread_cond_t c;
	int v;
};

static int bsem_init(struct bsem* b) {
	if (pthread_mutex_init(&b->m, NULL) < 0) {
		return -1;
	}

	if (pthread_cond_init(&b->c, NULL) < 0) {
		pthread_mutex_destroy(&b->m);
		return -1;
	}

	b->v = 0;
	return 0;
}

static void bsem_destroy(struct bsem* b) {
	pthread_mutex_destroy(&b->m);
	pthread_cond_destroy(&b->c);
}

int bsem_post(struct bsem* b) {
	if (pthread_mutex_lock(&b->m) < 0) {
		return -1;
	}
	b->v++;
	pthread_cond_signal(&b->c);
	pthread_mutex_unlock(&b->m);
	return 0;
}

static int bsem_wait(struct bsem* b) {
	if (pthread_mutex_lock(&b->m) < 0) {
		return -1;
	}
	while (!b->v) {
		pthread_cond_wait(&b->c, &b->m);
	}
	b->v--;
	pthread_mutex_unlock(&b->m);
	return 0;
}

struct threaddata
{
	/* The amount of threads per CPU */
	int threads;
	/* Semaphore to communicate that thread is started */
	struct bsem* sem;
	/* Result of the thread, should be equal to TOUCH when thread joins */
	size_t result;
	/* The amount of working memory to touch */
	size_t memory;
	/* The read socket to get the shared memory file descriptor */
	int socket;
};

static void err(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
	fflush(stderr);
}

/* Can't safely send shared memory file descriptors as integers
 * across processes, tho here it's safe on Linux since threads are not
 * really processes. The translation level is needed to be safe and does
 * not hurt on Linux.
 */
static int sendfd(int sock, int fd) {
	char buf[CMSG_SPACE(sizeof fd)];
	memset(buf, '\0', sizeof buf);

	struct iovec io = {
		.iov_base = "",
		.iov_len = 1
	};

	struct msghdr msg = {
		.msg_iov = &io,
		.msg_iovlen = 1,
		.msg_control = buf,
		.msg_controllen = sizeof buf
	};

	struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof fd);

	memmove(CMSG_DATA(cmsg), &fd, sizeof fd);

	msg.msg_controllen = cmsg->cmsg_len;

	return sendmsg(sock, &msg, 0) >= 0;
}

static int readfd(int sock) {
	char mbuf[1]; /* Message buffer */
	char cbuf[128]; /* Control buffer */

	struct iovec io = {
		.iov_base = mbuf,
		.iov_len = sizeof mbuf
	};

	struct msghdr msg = {
		.msg_iov = &io,
		.msg_iovlen = sizeof mbuf,
		.msg_control = cbuf,
		.msg_controllen = sizeof cbuf
	};

	if (recvmsg(sock, &msg, 0) < 0) {
		return -1;
	}

	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
	int fd;
	memmove(&fd, CMSG_DATA(cmsg), sizeof fd);
	return fd;
}

static int setcpuandprio(const char* which, int n) {
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(n, &set);
	if (pthread_setaffinity_np(pthread_self(), sizeof set, &set) < 0) {
		err("failed to set thread '%s' to cpu '%d'\n", which, n);
		return -1;
	}
	struct sched_param p = {
		.sched_priority = 99 /* Max priority */
	};
	if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &p) < 0) {
		err("failed to set thread '%s' priority", which);
		return -1;
	}
	return 0;
}

/* Functions to prevent the compiler from removing or reordering loads and stores
 * a function call behaves as a full memory barrier too
 */
static unsigned char __attribute__ ((noinline)) load(unsigned char *x) {
	return *x;
}

static void __attribute__ ((noinline)) store(unsigned char *x, unsigned char y) {
	*x = y;
}

/* Read thread */
static void* rd(void* opaque)
{
	struct threaddata *data = opaque;

	/* Set the CPU this thread will run on */
	if (setcpuandprio("rd", 0) < 0) {
		return NULL;
	}

	/* Get the shared memory file descriptor */
	int fd = readfd(data->socket);
	if (fcntl(fd, F_GETFD) < 0) {
		err("rd got invalid file descriptor for shared memory\n");
		goto error;
	}

	/* Map in shared memory */
	unsigned char* touch = mmap(0, data->memory, PROT_READ, MAP_SHARED, fd, 0);
	if (touch == MAP_FAILED) {
		err("failed to map shared memory for rd thread\n");
		goto error;
	}

	/* Inform the parent process we're ready */
	bsem_post(data->sem);

	/* Do a ton of reads */
	for (size_t i = 0; i < data->memory; i++) {
		load(touch + i);
		data->result++;
	}

	/* No longer need shared memory mapping */
	munmap(touch, data->memory);
	close(fd);

	return NULL;

error:
	if (fd) {
		close(fd);
	}
	/* Don't dead lock in main during initialization */
	bsem_post(data->sem);
	return NULL;
}

/* Write thread */
static void* wr(void* opaque)
{
	struct threaddata *data = opaque;

	/* Set the CPU this thread will run on */
	if (setcpuandprio("wr", data->threads) < 0) {
		return NULL;
	}

	/* Get the shared memory file descriptor */
	int fd = readfd(data->socket);
	if (fcntl(fd, F_GETFD) < 0) {
		err("wr got invalid file descriptor for shared memory\n");
		goto error;
	}

	/* Map in shared memory */
	unsigned char* touch = mmap(0, data->memory, PROT_WRITE, MAP_SHARED, fd, 0);
	if (touch == MAP_FAILED) {
		err("failed to map shared memory for wr thread\n");
		goto error;
	}

	/* Inform the parent process we're ready */
	bsem_post(data->sem);

	/* Do a ton of writes */
	for (size_t i = 0; i < data->memory; i++) {
		store(touch + i, i);
		data->result++;
	}

	/* No longer need shared memory mapping */
	munmap(touch, data->memory);
	close(fd);

	return NULL;

error:
	if (fd) {
		close(fd);
	}
	/* Don't dead lock in main during initialization */
	bsem_post(data->sem);
	return NULL;
}

/* High resolution timer */
static double gettime()
{
	struct timeval t;
	struct timezone tzp;
	gettimeofday(&t, &tzp);
	return t.tv_sec + t.tv_usec*1e-6;
}

/* Human readable size metric from size, not thread safe */
static const char* sizemetric(size_t size)
{
	static const char *sizes[] = { "B", "KiB", "MiB", "GiB", "TiB" };

	/* Find the suffix */
	double bytes = (float)size;
	size_t index = 0;
	for (; bytes >= 1024.0 && index < sizeof sizes / sizeof *sizes; index++ )
	{
		bytes /= 1024.0;
	}

	assert(index != sizeof sizes / sizeof *sizes);

	/* Truncate the representation if needed */
	char buffer[2*(DBL_MANT_DIG+DBL_MAX_EXP)];
	const int ret = snprintf(buffer, sizeof buffer, "%.*f", (int)sizeof buffer, bytes ); //-V512 truncation expected
	assert(ret > 0);
	/* Remove everything after (including) the period */
	char *period = strchr(buffer, '.');
	assert(period);
	period[3] = '\0';

	/* Format with human readable suffix */
	static char format[sizeof buffer + 5];
	snprintf(format, sizeof format, "%s %s", buffer, sizes[index] );
	return format;
}

static int enumeratecpus(void) {
	FILE* fp = popen("lscpu -p", "r");
	if (!fp) {
		return -1;
	}

	int maxcpus = 0;
	int maxcores = 0;
	char* linebuf = NULL;
	size_t linelen = 0;
	while (getline(&linebuf, &linelen, fp) != EOF) {
		if (*linebuf == '#') {
			continue;
		}
		int cpus = 0;
		int cores = 0;
		if (sscanf(linebuf, "%d,%d", &cpus, &cores) == 2) {
			if (cpus > maxcpus) {
				maxcpus = cpus;
			}
			if (cores > maxcores) {
				maxcores = cores;
			}
		}
	}
	free(linebuf);

	printf("discovered %d logcial cpus, %d physical, %d threads per core\n", maxcpus+1, maxcores+1, maxcpus/maxcores);
	fclose(fp);
	return maxcpus/maxcores;
}

static void usage(const char *app, FILE *fp) {
	fprintf(fp, "usage: %s [options]\n", app);
	fprintf(fp, "options:\n"
	            "  -s, --same          force read and write on the same cpu\n"
	            "  -h, --help          print this help message\n"
	            "  -m, --memory=MB     the amount of memory to work on in MB\n"
	            "  -t, --trials=COUNT  the amount of trials to run for benchmark\n");
}

static int isparam(int argc, char **argv, int *arg, char sh, const char *lng, char **argarg) {
	if (argv[*arg][0] != '-') {
		return 0;
	}
	/* short version */
	if (argv[*arg][1] == sh) {
		if (argv[*arg][2]) {
			*argarg = argv[*arg]+2;
			return 1;
		}
		++*arg;
		if (*arg == argc) {
			fprintf(stderr, "%s: option -%c requires an argument\n", argv[0], sh);
			usage(argv[0], stderr);
			*arg = -1;
			return 1;
		}
		*argarg = argv[*arg];
		return 1;
	}

	/* long version */
	if (argv[*arg][1] != '-') {
		return 0;
	}
	size_t len = strlen(lng);
	if (strncmp(argv[*arg]+2, lng, len)) {
		return 0;
	}
	if (argv[*arg][len+2] == '=') {
		*argarg = argv[*arg] + 3 + len;
		return 1;
	}
	if (!argv[*arg][len+2]) {
		++*arg;
		if (*arg == argc) {
			fprintf(stderr, "%s: option --%s requires an argument\n", argv[0], lng);
			usage(argv[0], stderr);
			*arg = -1;
			return 1;
		}
		*argarg = argv[*arg];
		return 1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	int trials = TRIALS;
	size_t memory = MEMORY;
	int same = 0;

	int arg = 1;
	for (; arg != argc; ++arg) {
		char *argarg = NULL;
		if (!strcmp(argv[arg], "-h") || !strcmp(argv[arg], "--help")) {
			usage(argv[0], stdout);
			return EXIT_SUCCESS;
		}
		if (!strcmp(argv[arg], "-s") || !strcmp(argv[arg], "--same")) {
			same = 1;
			continue;
		}
		if (isparam(argc, argv, &arg, 't', "trials", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			trials = atoi(argarg);
			continue;
		}
		if (isparam(argc, argv, &arg, 'm', "memory", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			memory = (size_t)atoi(argarg) * 1024ull * 1024ull;
			continue;
		}
		fprintf(stderr, "unknown option: %s\n", argv[arg]);
		usage(argv[0], stderr);
		return EXIT_FAILURE;
	}

	int threads = enumeratecpus();

	printf("measuing memory perf across CPUs with explicit memory mappings\n");
	printf("running %d trials on a space of %s\n", trials, sizemetric(memory));

	/* Create shared memory for each thread to utilize. We cannot use
	 * this processes memory because this process may be on the same
	 * core as the read or write thread. We want each thread to get its
	 * own mapping for it.
	 */
	int fd = shm_open("/xcpumemperf", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		err("failed to create shared memory\n");
		return EXIT_FAILURE;
	}
	/* No longer need the name */
	shm_unlink("/xcpumemperf");
	if (ftruncate(fd, memory) < 0) {
		err("failed to truncate shared memory\n");
		close(fd);
		return EXIT_FAILURE;
	}

	unsigned char* touch = mmap(0, memory, PROT_NONE, MAP_SHARED | MAP_POPULATE, fd, 0);
	if (touch == MAP_FAILED) {
		err("failed to map shared memory\n");
		close(fd);
		return EXIT_FAILURE;
	}

	/* Create a socket to communicate the shared memory file descriptor
	 * on, each thread will open the shared memory with its own mapping
	 * this way such that reads and writes behave through their own
	 * translation and not through the same virtual address this process
	 * gets.
	 */
	int pair[2];
	if (socketpair(PF_LOCAL, SOCK_STREAM, 0, pair) < 0) {
		err("failed to create socket pair for communicating\n");
		return EXIT_FAILURE;
	}

	pthread_t rdthr;
	struct bsem rdsem;
	pthread_t wrthr;
	struct bsem wrsem;

	if (bsem_init(&rdsem) < 0 || bsem_init(&wrsem) < 0) {
		err("failed to create semaphores for communicating\n");
		close(pair[0]);
		close(pair[1]);
		return EXIT_FAILURE;
	}

	struct threaddata rdthrd = { .threads = same ? 0 : threads, .memory = memory, .sem = &rdsem, .result = -1, .socket = pair[1] };
	struct threaddata wrthrd = { .threads = same ? 0 : threads, .memory = memory, .sem = &wrsem, .result = -1, .socket = pair[1] };

	/* Run trials */
	double tbeg = gettime();
	double rdtime = 0.0;
	double wrtime = 0.0;
	for (int i = 0; i < trials; i++)
	{
		double wrbeg = gettime();
		/* Create write thred */
		pthread_create(&wrthr, NULL, wr, &wrthrd);
		/* Write the shared memory file descriptor to the pipe */
		sendfd(pair[0], fd);
		/* Wait for the write thread to start */
		bsem_wait(&wrsem);

		double rdbeg = gettime();
		/* Create read thread */
		pthread_create(&rdthr, NULL, rd, &rdthrd);
		/* Write the shared memory file descriptor to the pipe */
		sendfd(pair[0], fd);
		/* Wait for the read thread to start */
		bsem_wait(&rdsem);

		/* Wait for write and read threads to complete */
		pthread_join(wrthr, NULL);
		double wrend = gettime();

		pthread_join(rdthr, NULL);
		double rdend = gettime();

		/* Calculate differences */
		double wrdif = wrend - wrbeg;
		double rddif = rdend - rdbeg;
		wrtime += wrdif;
		rdtime += rddif;

		printf("\rtrial %d of %d [%%% 3.2f] (wr %f sec, rd %f sec)", i+1, trials, (float)(i+1)/(float)trials*100.0f, wrdif, rddif);
		fflush(stdout);
	}
	printf("\n");
	double tend = gettime();

	bsem_destroy(&wrsem);
	bsem_destroy(&rdsem);

	/* Unmap shared memory */
	munmap(touch, memory);

	/* Close socket pair */
	close(pair[0]);
	close(pair[1]);

	if (rdthrd.result < memory - 1 || wrthrd.result < memory - 1) {
		err("incomplete results\n");
		return EXIT_FAILURE;
	}

	printf("finished average: (wr %f sec, rd %f sec) benchmark took %f secs total\n", wrtime/trials, rdtime/trials, tend-tbeg);
	return EXIT_SUCCESS;
}
