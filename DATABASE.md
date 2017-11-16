# Comparison of various CPU(s)

## How to compare these results

You cannot make a comparison between runs with different amounts of threads
used per trial run. At least not directly. Remember that two threads are
spawned per thread trial run (a read thread and a write thread) so when
you see `N thread(s) per trial run` there is actually `N*2` threads being
run in total for that one trial. More threads will take longer even if
the threads are all scheduled at the same time, the memory has to be
touched more which in turn makes it slower. In general, if the configuration
has more physical cores it will be slower than say a really old system
with only a few.

However, fret not. It's possible to make the comparison between these
systems by taking a look at the amount of memory benched. For instance
if one configuration benched 16 GiB while another only benched 2 GiB,
then the difference between them is a factor of 8. Taking this into
account you need to multiply the 16 GiB's speed by 8 to compare it to
the lesser system for throughput, similarly you'd multiply the lesser
configuration's time by 8 to scale it to compare relative speed.

## Configurations

The results of the configurations are organized by the amount of threads
used per trial run to make comparing the results much easier. Tho
you can still compare results against different threads per trial run
counts as described above.

The following are timings from various CPUs with the default options
of `xcpumempref`.

You can contribute your timings here by submitting a pull request. The
more configurations we have the better it'll be to provide some useful
graphs in the future.

### 8 Thread Configurations

#### i7-4790k
```
discovered Intel(R) Core(TM) i7-4790K CPU @ 4.00GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.215773 sec, rd 1.952435 sec)
thread pair averages:
  1 (wr 3.090100 sec, rd 2.877040 sec)
  2 (wr 2.714553 sec, rd 2.499464 sec)
  3 (wr 2.302922 sec, rd 2.090448 sec)
  4 (wr 1.888244 sec, rd 1.711494 sec)
  5 (wr 1.510761 sec, rd 1.291384 sec)
  6 (wr 1.107649 sec, rd 0.909908 sec)
  7 (wr 0.745253 sec, rd 0.559212 sec)
  8 (wr 0.358107 sec, rd 0.158068 sec)
total average: (wr 1.714699 sec, rd 1.512127 sec)
benched 32.00 GiB worth of memory (327.67 MiB/s) in 99.197525 secs total
```
