#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "thread.h"
#include "util.h"
#include "msg.h"

/* How many times to run the benchmark (Default) */
#define TRIALS 128

/* The benchmark memory to operate on (Default) */
#define MEMORY (size_t)(32ull * 1024ull * 1024ull) // 32MiB

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

	struct cpuinfo info;
	if (util_getcpuinfo(&info) < 0) {
		fprintf(stderr, "failed to query cpu information\n");
		return EXIT_FAILURE;
	}

	printf("discovered %d logcial cpus, %d physical, %d threads per core\n", info.logical, info.physical, info.threads);
	printf("measuing memory perf across CPUs with explicit memory mappings\n");
	printf("running %d trials on a space of %s\n", trials, util_humansize(memory));

	int threads = info.threads;

	/* Create shared memory for each thread to utilize. We cannot use
	 * this processes memory because this process may be on the same
	 * core as the read or write thread. We want each thread to get its
	 * own mapping for it.
	 */
	int fd = shm_open("/xcpumemperf", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		fprintf(stderr, "failed to create shared memory\n");
		return EXIT_FAILURE;
	}
	/* No longer need the name */
	shm_unlink("/xcpumemperf");
	if (ftruncate(fd, memory) < 0) {
		fprintf(stderr, "failed to truncate shared memory\n");
		close(fd);
		return EXIT_FAILURE;
	}

	unsigned char* touch = mmap(0, memory, PROT_NONE, MAP_SHARED | MAP_POPULATE, fd, 0);
	if (touch == MAP_FAILED) {
		fprintf(stderr, "failed to map shared memory\n");
		close(fd);
		return EXIT_FAILURE;
	}

	struct msg m;
	if (msg_init(&m) < 0) {
		fprintf(stderr, "failed to create message channel for communicating\n");
		return EXIT_FAILURE;
	}

	struct thread wr;
	struct thread rd;

	int wrt = 0;
	int rdt = same ? threads : wrt;

	double tbeg = util_gettime();

	double wrtime = 0.0;
	double rdtime = 0.0;
	for (int i = 0; i < trials; i++)
	{
		double wrbeg = util_gettime();
		if (thread_init(&wr, wrt, WR, memory, &m) < 0) {
			fprintf(stderr, "failed to create wr thread\n");
			return EXIT_FAILURE;
		}
		thread_wait(&wr, fd);

		double rdbeg = util_gettime();
		if (thread_init(&rd, rdt, RD, memory, &m) < 0) {
			fprintf(stderr, "failed to create rd thread\n");
			return EXIT_FAILURE;
		}
		thread_wait(&rd, fd);

		thread_join(&wr);
		double wrend = util_gettime();

		thread_join(&rd);
		double rdend = util_gettime();

		/* Calculate differences */
		double wrdif = wrend - wrbeg;
		double rddif = rdend - rdbeg;
		wrtime += wrdif;
		rdtime += rddif;

		printf("\rtrial %d of %d [%%% 3.2f] (wr %f sec, rd %f sec)", i+1, trials, (float)(i+1)/(float)trials*100.0f, wrdif, rddif);
		fflush(stdout);

		thread_destroy(&wr);
		thread_destroy(&rd);
	}

	double tend = util_gettime();
	printf("\n");

	/* Unmap shared memory */
	munmap(touch, memory);

	/* Close message channel */
	msg_destroy(&m);

	printf("finished average: (wr %f sec, rd %f sec) benchmark took %f secs total\n", wrtime/trials, rdtime/trials, tend-tbeg);
	return EXIT_SUCCESS;
}
