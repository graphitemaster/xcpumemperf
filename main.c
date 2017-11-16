#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "thread.h"
#include "util.h"
#include "msg.h"

/* How many times to run the benchmark (Default) */
#define TRIALS 32

/* The benchmark memory to operate on (Default) */
#define MEMORY (size_t)(128ull * 1024ull * 1024ull) // 128MiB

static void usage(const char *app, FILE *fp) {
	fprintf(fp, "usage: %s [options]\n", app);
	fprintf(fp, "options:\n"
	            "  -h, --help                     print this help message\n"
	            "  -T, --threads=COUNT            the amount of threads to use per trial run\n"
	            "  -m, --memory=MB                the amount of memory to work on in MiB\n"
	            "  -t, --trials=COUNT             the amount of trials to run for benchmark\n"
	            "  -F, --force_same_cpu=OPTION    forces read and write pairs to end up on the same CPU\n"
	            "  -p, --populate=OPTION          populate shared memory mapping before benching\n");
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
	int threads = -1;
	int force_same_cpu = 0;
	int populate = 0;
	size_t memory = MEMORY;

	int arg = 1;
	for (; arg != argc; ++arg) {
		char *argarg = NULL;
		if (!strcmp(argv[arg], "-h") || !strcmp(argv[arg], "--help")) {
			usage(argv[0], stdout);
			return EXIT_SUCCESS;
		}
		if (isparam(argc, argv, &arg, 'F', "force-same-cpu", &argarg)) {
			if (arg < 0) {
				return EXIT_FAILURE;
			}
			force_same_cpu = argarg ? atoi(argarg) : 1;
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
		threads = info.physical;
	}

	char memfmt[1024];
	printf("discovered %s: %d logical CPU(s), %d physical, %d thread(s) per core\n", info.name, info.logical, info.physical, info.threads);
	printf("measuring memory perf across CPU(s) with explicit memory mappings\n");
	printf("running %d trial(s) on a space of %s with %d thread(s) per trial run\n", trials, util_humansize(memfmt, sizeof memfmt, memory), threads);
	if (force_same_cpu) {
		printf("forcing read and writes on same physical CPU(s)\n");
	}
	if (populate) {
		printf("populating shared memory map for benchmark\n");
	}

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

	struct task *wr = calloc(sizeof *wr, threads);
	if (!wr) {
		goto oom;
	}
	struct task *rd = calloc(sizeof *rd, threads);
	if (!rd) {
		goto oom;
	}

	double wrtime = 0.0;
	double rdtime = 0.0;

	double tbeg = util_gettime();

	for (int i = 0; i < trials; i++)
	{
		/* Execute benchmarks on all threads for this trial */
		for (int thread = 0; thread < threads; thread++)
		{
			int wrcpu = thread % info.logical;

			/*
			 * Note we assume that the current core index + how ever many threads per core - 1 will force the
			 * reading thread onto another CPU. Or put another way, [0, n] where n is the # of threads per
			 * core we assume to be the same CPU. This is true for almost all binned configurations but may
			 * be different in NUMA systems.
			 */
			int rdcpu = force_same_cpu ? wrcpu : (thread + info.threads - 1) % info.logical;

			/* Bring up write thread */
			wr[thread].beg = util_gettime();
			if (thread_init(&wr[thread].thread, wrcpu, WR, memory, &m) < 0) {
				fprintf(stderr, "failed to create wr thread\n");
				return EXIT_FAILURE;
			}
			thread_wait(&wr[thread].thread, fd);

			/* Bring up read thread */
			rd[thread].beg = util_gettime();
			if (thread_init(&rd[thread].thread, rdcpu, RD, memory, &m) < 0) {
				fprintf(stderr, "failed to create rd thread\n");
				return EXIT_FAILURE;
			}
			thread_wait(&rd[thread].thread, fd);
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
			wrtime += wr[thread].dif;
			rdtime += rd[thread].dif;

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

		printf("\rtrial %d of %d [%%%3.2f] (wr %f sec, rd %f sec)", i+1, trials, (float)(i+1)/(float)trials*100.0f, wrdif, rddif);
		fflush(stdout);
	}

	double tend = util_gettime();
	printf("\n");

	/* Unmap shared memory */
	munmap(touch, memory);

	/* Close message channel */
	msg_destroy(&m);

	printf("thread averages:\n");
	for (int thread = 0; thread < threads; thread++) {
		int strides = threads*trials;
		printf("  %d (wr %f sec, rd %f sec)\n", thread+1, wr[thread].avg/strides, rd[thread].avg/strides);
	}

	double time = tend-tbeg;
	size_t size = threads*trials*memory;
	size_t size_sec = size / (size_t)ceil(time);
	char sizetfmt[1024];
	char sizesfmt[1024];
	printf("total average: (wr %f sec, rd %f sec)\n", wrtime/trials, rdtime/trials);
	printf("benched %s worth of memory (%s/s) in %f secs total\n", util_humansize(sizetfmt, sizeof sizetfmt, size), util_humansize(sizesfmt, sizeof sizesfmt, size_sec), time);

	free(wr);
	free(rd);

	return EXIT_SUCCESS;

oom:
	fprintf(stderr, "Out of memory");

	free(wr);
	free(rd);

	munmap(touch, memory);
	msg_destroy(&m);

	return EXIT_FAILURE;
}
