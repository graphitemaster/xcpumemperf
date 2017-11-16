#ifndef UTIL_HDR
#define UTIL_HDR

/* High resolution timer wall time in seconds */
double util_gettime(void);

/* Human readable size metric from size */
const char* util_humansize(char* buffer, size_t bufsize, size_t size);

struct cpuinfo
{
	struct { int cpu, core; } entries[128];
	int physical;
	int threads;
	int logical;
	char name[1024];
};

int util_getcpuinfo(struct cpuinfo *info);

int util_enumeratecpus(void);
#endif
