#include <stdio.h>
#include <float.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <sys/time.h>

#include "util.h"

double util_gettime(void)
{
	struct timeval t;
	struct timezone tzp;
	gettimeofday(&t, &tzp);
	return t.tv_sec + t.tv_usec*1e-6;
}

const char* util_humansize(char *fmtbuffer, size_t bufsize, size_t size)
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
	snprintf(fmtbuffer, bufsize, "%s %s", buffer, sizes[index]);
	return fmtbuffer;
}

int util_getcpuinfo(struct cpuinfo* info) {
	FILE* fp = popen("lscpu --parse=CPU,CORE,ONLINE", "r");
	if (!fp) {
		return -1;
	}

	/* There's always at least one logical */
	info->logical = 0;

	/* There's always at least on physical */
	int physical = 1;

	/* Read the output of lscpu */
	char line[LINE_MAX];
	while (fgets(line, LINE_MAX, fp)) {
		if (line[0] == '#') {
			continue;
		}
		int cpu = 0;
		int core = 0;
		char online = '\0';
		if (sscanf(line, "%d,%d,%c", &cpu, &core, &online) == 3) {
			if (online == 'Y') {
				info->entries[info->logical].cpu = cpu;
				info->entries[info->logical].core = core;
				info->logical++;
				if (core >= physical) {
					physical = core + 1;
				}
			}
		} else {
			return -1;
		}
	}

	/*
	 * The amount of threads per core is the amount of logical ones divided
	 * by the amount of cores (physical). This is not exactly robust against
	 * strange NUMA configurations but is correct for the most part.
	 */
	info->threads = info->logical / physical;
	info->physical = physical;
	return 0;
}
