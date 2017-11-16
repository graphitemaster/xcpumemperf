# Comparison of various CPU(s)

## How to read these results

The first line of the output is the detected CPU and it's topology, in
particular the amount of logical threads it has, followed by the amount
of physical cores it has and if a core supports multiple threads (also
known as SMT (Simultanerous multithreading))

The line containing trials tells you how many times the test is being
done, more trials results in more stable results to reduce scheduling
noise and overhead of thread construction and other sources of noise.
The space refers to the memory pool which will be mapped twice for
each thread pair (read and write threads in a pair and each get their
own virtual mapping for.) This space is continuously touched by both
processes while the system is under load to get an idea of cross CPU
memory performance. The thread pair is how many pairs of threads (it's
a pair because there is a thread for read and a thread for write) are
to be used in the trial. The final count is just the amount of actual
threads will be used in total for the run (it's always two times the
amount of thread pairs.)

The next line shows you how threads are allocated for the read and
write pairs, each `[]` is a pair and the first number in a pair is the
logical CPU which will be used for reading while the second number is
the logical CPU which will be used for writing. These are staggered
because we want reads and writes for a mapping to happen across CPUs.
This staggering is controlled by the amount of threads per core. You
can disable staggering with -F or --force-same-cpu.

The rest of the output is a list of running average seconds on each
thread pair that carried out the trials. This output serves to show how
the top most pairs tend to be the slowest while the bottom most ones are
faster, which proves that memory bandwidth scales with the thread count.
If you see values at the bottom of the list having averages higer than
the top then you've hit a pathological case where more threads makes
memory slower.

The final line is what can be used for comparison, in particular it tells
you how much memory was touched through the course of all trials for
all thread pairs and what the memory throughput of that was as well as
how long it took to benchmark everything. Systems with more threads will
take longer to benchmark but should have relatively stable memory
throughput.

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

### 56 Thread Configurations

#### Xeon E5-2683
```
discovered Intel(R) Xeon(R) CPU E5-2683 v3 @ 2.00GHz: 56 logical CPU(s), 28 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 56 thread pair(s) or 112 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 16] [15, 17] [16, 18] [17, 19] [18, 20] [19, 21] [20, 22] [21, 23] [22, 24] [23, 25] [24, 26] [25, 27] [26, 28] [27, 29] [28, 30] [29, 31] [30, 32] [31, 33] [32, 34] [33, 35] [34, 36] [35, 37] [36, 38] [37, 39] [38, 40] [39, 41] [40, 42] [41, 43] [42, 44] [43, 45] [44, 46] [45, 47] [46, 48] [47, 49] [48, 50] [49, 51] [50, 52] [51, 53] [52, 54] [53, 55] [54, 0] [55, 1]
trial 32 of 32 [%100.00] (wr 3.356061 sec, rd 3.292895 sec)
thread pair averages:
  1 (wr 6.551104 sec, rd 6.464674 sec)
  2 (wr 6.386525 sec, rd 6.311538 sec)
  3 (wr 6.235609 sec, rd 6.163323 sec)
  4 (wr 6.087573 sec, rd 6.012213 sec)
  5 (wr 5.937825 sec, rd 5.865735 sec)
  6 (wr 5.793511 sec, rd 5.720618 sec)
  7 (wr 5.647209 sec, rd 5.571719 sec)
  8 (wr 5.502309 sec, rd 5.428147 sec)
  9 (wr 5.359774 sec, rd 5.286072 sec)
  10 (wr 5.215148 sec, rd 5.143750 sec)
  11 (wr 5.072181 sec, rd 4.999133 sec)
  12 (wr 4.927767 sec, rd 4.855642 sec)
  13 (wr 4.782368 sec, rd 4.715937 sec)
  14 (wr 4.643525 sec, rd 4.569659 sec)
  15 (wr 4.497669 sec, rd 4.428248 sec)
  16 (wr 4.360314 sec, rd 4.288489 sec)
  17 (wr 4.217381 sec, rd 4.145677 sec)
  18 (wr 4.073193 sec, rd 4.001037 sec)
  19 (wr 3.926830 sec, rd 3.858657 sec)
  20 (wr 3.787214 sec, rd 3.715755 sec)
  21 (wr 3.644725 sec, rd 3.576374 sec)
  22 (wr 3.511026 sec, rd 3.440647 sec)
  23 (wr 3.374022 sec, rd 3.306523 sec)
  24 (wr 3.240768 sec, rd 3.178481 sec)
  25 (wr 3.118372 sec, rd 3.054122 sec)
  26 (wr 2.992118 sec, rd 2.935895 sec)
  27 (wr 2.876537 sec, rd 2.817127 sec)
  28 (wr 2.757947 sec, rd 2.694673 sec)
  29 (wr 2.629678 sec, rd 2.565643 sec)
  30 (wr 2.501914 sec, rd 2.447638 sec)
  31 (wr 2.382387 sec, rd 2.325503 sec)
  32 (wr 2.269683 sec, rd 2.219034 sec)
  33 (wr 2.166883 sec, rd 2.119535 sec)
  34 (wr 2.074433 sec, rd 2.025925 sec)
  35 (wr 1.977334 sec, rd 1.936154 sec)
  36 (wr 1.896815 sec, rd 1.853737 sec)
  37 (wr 1.816589 sec, rd 1.778170 sec)
  38 (wr 1.743602 sec, rd 1.707488 sec)
  39 (wr 1.675639 sec, rd 1.639335 sec)
  40 (wr 1.600802 sec, rd 1.562048 sec)
  41 (wr 1.527216 sec, rd 1.490129 sec)
  42 (wr 1.453425 sec, rd 1.416182 sec)
  43 (wr 1.377558 sec, rd 1.338594 sec)
  44 (wr 1.296273 sec, rd 1.251356 sec)
  45 (wr 1.205590 sec, rd 1.162892 sec)
  46 (wr 1.115205 sec, rd 1.071499 sec)
  47 (wr 1.017554 sec, rd 0.965636 sec)
  48 (wr 0.918009 sec, rd 0.869780 sec)
  49 (wr 0.819553 sec, rd 0.767531 sec)
  50 (wr 0.717685 sec, rd 0.671209 sec)
  51 (wr 0.620721 sec, rd 0.569476 sec)
  52 (wr 0.518073 sec, rd 0.465705 sec)
  53 (wr 0.415801 sec, rd 0.363586 sec)
  54 (wr 0.311502 sec, rd 0.260475 sec)
  55 (wr 0.208615 sec, rd 0.157547 sec)
  56 (wr 0.105606 sec, rd 0.052532 sec)
total average: (wr 2.980084 sec, rd 2.921504 sec)
benched 224.00 GiB worth of memory (1.06 GiB/s) in 209.909727 secs total
```
