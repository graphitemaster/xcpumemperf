#ifndef UTIL_HDR
#define UTIL_HDR

/* High resolution timer wall time in seconds */
double util_gettime(void);

/* Human readable size metric from size, not thread safe */
const char* util_humansize(size_t size);

struct cpuinfo
{
	struct { int cpu, core; } entries[128];
	int physical;
	int threads;
	int logical;
};

int util_getcpuinfo(struct cpuinfo *info);

int util_enumeratecpus(void);
#endif
