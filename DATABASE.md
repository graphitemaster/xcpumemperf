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
#### i7-4710HQ
```
discovered Intel(R) Core(TM) i7-4710HQ CPU @ 2.50GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.353468 sec, rd 2.094124 sec)
thread pair averages:
  1 (wr 3.443859 sec, rd 3.184389 sec)
  2 (wr 2.945995 sec, rd 2.712724 sec)
  3 (wr 2.482770 sec, rd 2.231690 sec)
  4 (wr 2.018252 sec, rd 1.808466 sec)
  5 (wr 1.637089 sec, rd 1.448504 sec)
  6 (wr 1.253560 sec, rd 1.030586 sec)
  7 (wr 0.843618 sec, rd 0.626392 sec)
  8 (wr 0.426604 sec, rd 0.203752 sec)
total average: (wr 1.881468 sec, rd 1.655813 sec)
benched 32.00 GiB worth of memory (295.20 MiB/s) in 110.436308 secs total
```

### 16 Thread Configurations

#### Ryzen 1700
```
discovered AMD Ryzen 7 1700 Eight-Core Processor: 16 logical CPU(s), 8 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 16 thread pair(s) or 32 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 0] [15, 1]
trial 32 of 32 [%100.00] (wr 4.253188 sec, rd 3.976050 sec)
thread pair averages:
  1 (wr 6.426915 sec, rd 6.197229 sec)
  2 (wr 5.976421 sec, rd 5.777716 sec)
  3 (wr 5.566702 sec, rd 5.375356 sec)
  4 (wr 5.179495 sec, rd 4.987682 sec)
  5 (wr 4.792211 sec, rd 4.617038 sec)
  6 (wr 4.425688 sec, rd 4.220604 sec)
  7 (wr 4.029085 sec, rd 3.854654 sec)
  8 (wr 3.634878 sec, rd 3.422709 sec)
  9 (wr 3.198352 sec, rd 2.997448 sec)
  10 (wr 2.806219 sec, rd 2.562666 sec)
  11 (wr 2.351325 sec, rd 2.148494 sec)
  12 (wr 1.962674 sec, rd 1.764082 sec)
  13 (wr 1.567649 sec, rd 1.371088 sec)
  14 (wr 1.181451 sec, rd 0.978599 sec)
  15 (wr 0.780773 sec, rd 0.593060 sec)
  16 (wr 0.394149 sec, rd 0.201778 sec)
total average: (wr 3.392124 sec, rd 3.191888 sec)
benched 64.00 GiB worth of memory (318.13 MiB/s) in 205.969100 secs total
```
