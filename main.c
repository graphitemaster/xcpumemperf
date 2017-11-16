#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "thread.h"
#include "util.h"
#include "msg.h"

/* How many times to run the benchmark (Default) */
#define TRIALS 32

/* The benchmark memory to operate on (Default) */
#define MEMORY (size_t)(128ull * 1024ull * 1024ull) // 128MiB

#define SHMNAME "/xcpumemperf"
#define SHAREFILE "/tmp/xcpumemperf.result"

static FILE* share;

static void usage(const char *app, FILE *fp) {
	fprintf(fp, "usage: %s [options]\n", app);
	fprintf(fp, "options:\n"
	            "  -h, --help            print this help message\n"
	            "  -T, --threads=COUNT   the amount of thread pairs to use per trial run\n"
	            "  -m, --memory=MB       the amount of memory to work on in MiB\n"
	            "  -t, --trials=COUNT    the amount of trials to run for benchmark\n"
	            "  -F, --force-same-cpu  forces read and write pairs to end up on the same CPU\n"
	            "  -s, --share           share results by posting output to sprunge\n"
	            "  -p, --populate        populate shared memory mapping before benching\n"
	            "  -H, --hugepage=DIR    create a huge page using this directory (must be a hugepages mountpoint)\n");
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

static void out(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	vfprintf(stdout, fmt, va);
	va_end(va);
	if (share) {
		va_start(va, fmt);
		vfprintf(share, fmt, va);
		va_end(va);
	}
}

int main(int argc, char **argv)
{
	int trials = TRIALS;
	int threads = -1;
	int force_same_cpu = 0;
	int populate = 0;
	size_t memory = MEMORY;
	int result = EXIT_FAILURE;
	char hugepage_path[PATH_MAX] = { 0 };

	int arg = 1;
	for (; arg != argc; ++arg) {
		char *argarg = NULL;
		if (!strcmp(argv[arg], "-h") || !strcmp(argv[arg], "--help")) {
			usage(argv[0], stdout);
			return EXIT_SUCCESS;
		}
		if (!strcmp(argv[arg], "-s") || !strcmp(argv[arg], "--share")) {
			share = fopen(SHAREFILE, "w");
			continue;
		}
		if (!strcmp(argv[arg], "-F") || !strcmp(argv[arg], "--force-same-cpu")) {
			force_same_cpu = 1;
			continue;
		}
		if (isparam(argc, argv, &arg, 'p', "populate", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			populate = argarg ? atoi(argarg) : 1;
			continue;
		}
		if (isparam(argc, argv, &arg, 't', "trials", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			trials = atoi(argarg);
			continue;
		}
		if (isparam(argc, argv, &arg, 'T', "threads", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			if (argarg) {
				threads = atoi(argarg);
			}
			continue;
		}
		if (isparam(argc, argv, &arg, 'm', "memory", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			memory = (size_t)atoi(argarg) * 1024ull * 1024ull;
			continue;
		}
		if (isparam(argc, argv, &arg, 'H', "hugepage", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			if (argarg[0]) {
				(void)snprintf(hugepage_path, sizeof hugepage_path, "%s" SHMNAME ".%d", argarg, getpid());
			} else {
				hugepage_path[0] = 0;
			}
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

	/* Have as many threads as there are physical cores by default */
	if (threads == -1) {
		threads = info.logical;
	}

	if (threads >= info.logical) {
		printf("warning the following benchmark will saturate your CPU with the selected configuration of %d threads\n", threads);
		printf("do you want to continue? [y/N] ");
		fflush(stdout);
		int ch = getchar();
		if (!strchr("\nyY", ch)) {
			return EXIT_SUCCESS;
		}
		printf("\n");
	}

	char memfmt[1024];
	out("discovered %s: %d logical CPU(s), %d physical, %d thread(s) per core\n", info.name, info.logical, info.physical, info.threads);
	out("measuring memory perf across CPU(s) with explicit memory mappings\n");
	out("running %d trial(s) on a space of %s with %d thread pair(s) or %d thread(s) total per trial run\n", trials, util_humansize(memfmt, sizeof memfmt, memory), threads, threads*2);
	if (force_same_cpu) {
		out("forcing read and writes on same physical CPU(s)\n");
	}
	if (populate) {
		out("populating shared memory map for benchmark\n");
	}

	/* Create shared memory for each thread to utilize. We cannot use
	 * this processes memory because this process may be on the same
	 * core as the read or write thread. We want each thread to get its
	 * own mapping for it.
	 */
	int fd = -1;
	if (hugepage_path[0]) {
		fd = open(hugepage_path, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	} else {
		fd = shm_open(SHMNAME, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	}
	if (fd < 0) {
		fprintf(stderr, "failed to create shared memory\n");
		return EXIT_FAILURE;
	}
	/* No longer need the name */
	if (hugepage_path[0]) {
		unlink(hugepage_path);
	} else {
		shm_unlink(SHMNAME);
	}
	if (ftruncate(fd, memory) < 0) {
		fprintf(stderr, "failed to truncate shared memory\n");
		close(fd);
		return EXIT_FAILURE;
	}

	unsigned char* touch = mmap(0, memory, PROT_NONE, MAP_SHARED | (populate ? MAP_POPULATE : 0), fd, 0);
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

	struct task {
		struct thread thread;
		double beg; /* Time when task started */
		double end; /* Time when task ended */
		double dif; /* Last difference between task completion */
		double avg; /* Running average of task differences */
		int complete; /* Indicates if task is completed */
	};

	struct pair {
		int wr;
		int rd;
	};

	struct pair *pairs = NULL;
	struct task *wr = NULL;
	struct task *rd = NULL;
	wr = calloc(sizeof *wr, threads);
	if (!wr) {
		fprintf(stderr, "Out of memory");
		goto cleanup;
	}
	rd = calloc(sizeof *rd, threads);
	if (!rd) {
		fprintf(stderr, "Out of memory");
		goto cleanup;
	}
	pairs = calloc(sizeof *pairs, threads);
	if (!pairs) {
		fprintf(stderr, "Out of memory");
		goto cleanup;
	}

	/* Output thread pairs */
	out("thread cpu pairs: ");
	for (int thread = 0; thread < threads; thread++)
	{
		/*
		 * Note we assume that the current core index + how ever many threads per core - 1 will force the
		 * reading thread onto another CPU. Or put another way, [0, n] where n is the # of threads per
		 * core we assume to be the same CPU. This is true for almost all binned configurations but may
		 * be different in NUMA systems.
		 */
		int wrcpu = thread % info.logical;
		int rdcpu = -1;
		if (force_same_cpu) {
			rdcpu = wrcpu;
		} else {
			int offset = wrcpu + info.threads;
			if (offset >= info.logical) {
				rdcpu = offset % info.logical;
			} else {
				rdcpu = offset;
			}
		}

		out("[%d, %d] ", wrcpu, rdcpu);
		pairs[thread].wr = wrcpu;
		pairs[thread].rd = rdcpu;
	}
	out("\n");

	double wrtime = 0.0;
	double rdtime = 0.0;

	double tbeg = util_gettime();

	for (int i = 0; i < trials; i++)
	{
		/* Execute benchmarks on all threads for this trial */
		for (int thread = 0; thread < threads; thread++)
		{
			/* Bring up write thread */
			wr[thread].beg = util_gettime();
			if (thread_init(&wr[thread].thread, pairs[thread].wr, WR, memory, &m) < 0) {
				fprintf(stderr, "failed to create wr thread\n");
				return EXIT_FAILURE;
			}
			thread_wait(&wr[thread].thread, fd);

			/* Bring up read thread */
			rd[thread].beg = util_gettime();
			if (thread_init(&rd[thread].thread, pairs[thread].rd, RD, memory, &m) < 0) {
				fprintf(stderr, "failed to create rd thread\n");
				return EXIT_FAILURE;
			}
			thread_wait(&rd[thread].thread, fd);
		}

		/* Begin benchmarking on those threads */
		for (int thread = 0; thread < threads; thread++) {
			thread_benchmark(&wr[thread].thread);
			thread_benchmark(&rd[thread].thread);
		}

		/* Wait for threads to complete in this trial */
		int complete = 0;
		while (complete < threads*2) {
			for (int thread = 0; thread < threads; thread++)
			{
				if (!wr[thread].complete && thread_join(&wr[thread].thread) >= 0) {
					wr[thread].end = util_gettime();
					wr[thread].complete = 1;
					complete++;
				}

				if (!rd[thread].complete && thread_join(&rd[thread].thread) >= 0) {
					rd[thread].end = util_gettime();
					rd[thread].complete = 1;
					complete++;
				}
			}
		}

		/* Average difference across the threads in this trial */
		double wrdif = 0.0;
		double rddif = 0.0;
		for (int thread = 0; thread < threads; thread++) {
			/* Averages difference for all trial runs */
			wr[thread].dif = wr[thread].end - wr[thread].beg;
			rd[thread].dif = rd[thread].end - rd[thread].beg;
			wr[thread].avg += wr[thread].dif;
			rd[thread].avg += rd[thread].dif;

			/* The averaged difference for this trial run */
			wrdif += wr[thread].dif;
			rddif += rd[thread].dif;

			/* Reset completion status for next trial run */
			wr[thread].complete = 0;
			rd[thread].complete = 0;

			/* Reset threads for next trial run */
			thread_destroy(&wr[thread].thread);
			thread_destroy(&rd[thread].thread);
		}
		wrdif /= threads;
		rddif /= threads;
		wrtime += wrdif;
		rdtime += rddif;

		printf("\rtrial %d of %d [%%%3.2f] (wr %f sec, rd %f sec)", i+1, trials, (float)(i+1)/(float)trials*100.0f, wrdif, rddif);
		fflush(stdout);

		/* Emit the last one to the share log if specified */
		if (share && i == trials - 1) {
			fprintf(share, "trial %d of %d [%%%3.2f] (wr %f sec, rd %f sec)\n", i+1, trials, (float)(i+1)/(float)trials*100.0f, wrdif, rddif);
		}
	}

	double tend = util_gettime();
	printf("\n");

	out("thread pair averages:\n");
	for (int thread = 0; thread < threads; thread++) {
		out("  %d (wr %f sec, rd %f sec)\n", thread+1, wr[thread].avg/trials, rd[thread].avg/trials);
	}

	double time = tend-tbeg;
	size_t size = threads*trials*memory;
	size_t size_sec = size / (size_t)ceil(time);
	char sizetfmt[1024];
	char sizesfmt[1024];
	out("total average: (wr %f sec, rd %f sec)\n", wrtime/trials, rdtime/trials);
	out("benched %s worth of memory (%s/s) in %f secs total\n", util_humansize(sizetfmt, sizeof sizetfmt, size), util_humansize(sizesfmt, sizeof sizesfmt, size_sec), time);

	if (share) {
		printf("share your result: ");
		fflush(stdout);
		fclose(share);
		share = NULL;
		system("cat " SHAREFILE " | curl -F 'sprunge=<-' http://sprunge.us");
		unlink(SHAREFILE);
	}

	result = EXIT_SUCCESS;

cleanup:
	free(wr);
	free(rd);
	free(pairs);

	munmap(touch, memory);
	msg_destroy(&m);

	if (share) {
		fclose(share);
	}

	return result;
}
