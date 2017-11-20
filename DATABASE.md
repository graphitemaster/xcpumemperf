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
can disable staggering with `-F` or `--force-same-cpu.`

The rest of the output is a list of running average seconds on each
thread pair that carried out the trials. It's a running average so
it'll always decrease in time since it's relative to the previous
trials. The distance between steps in each run should be proportional
to how many trials have already been completed. You can think of it
as the delta amount of time relative to all other thread trials before
it.

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

### 4 Thread Configurations

#### i5-2500
```
discovered Intel(R) Core(TM) i5-2500 CPU @ 3.30GHz: 4 logical CPU(s), 4 physical, 1 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 4 thread pair(s) or 8 thread(s) total per trial run
thread cpu pairs: [0, 1] [1, 2] [2, 3] [3, 0]
trial 32 of 32 [%100.00] (wr 2.234547 sec, rd 1.833982 sec)
thread pair averages:
  1 (wr 1.621019 sec, rd 1.396586 sec)
  2 (wr 1.199550 sec, rd 0.987120 sec)
  3 (wr 0.757112 sec, rd 0.555062 sec)
  4 (wr 0.366981 sec, rd 0.193971 sec)
total average: (wr 0.986165 sec, rd 0.783185 sec)
benched 16.00 GiB worth of memory (309.13 MiB/s) in 52.042662 secs total
```

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

```
discovered Intel(R) Core(TM) i7-4790K CPU @ 4.00GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 1.927222 sec, rd 1.673217 sec)
thread pair averages:
  1 (wr 2.765958 sec, rd 2.556524 sec)
  2 (wr 2.391176 sec, rd 2.218927 sec)
  3 (wr 2.046278 sec, rd 1.885729 sec)
  4 (wr 1.701208 sec, rd 1.527384 sec)
  5 (wr 1.370657 sec, rd 1.216602 sec)
  6 (wr 1.049935 sec, rd 0.886198 sec)
  7 (wr 0.689024 sec, rd 0.519007 sec)
  8 (wr 0.345747 sec, rd 0.163344 sec)
total average: (wr 1.544998 sec, rd 1.371714 sec)
benched 32.00 GiB worth of memory (368.17 MiB/s) in 88.740121 secs total
```

#### i7-4700MQ
```
discovered Intel(R) Core(TM) i7-4700MQ CPU @ 2.40GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.855616 sec, rd 2.549412 sec)
thread pair averages:
  1 (wr 3.483890 sec, rd 3.249389 sec)
  2 (wr 3.021255 sec, rd 2.817133 sec)
  3 (wr 2.597532 sec, rd 2.373908 sec)
  4 (wr 2.160271 sec, rd 1.942598 sec)
  5 (wr 1.724226 sec, rd 1.505634 sec)
  6 (wr 1.283305 sec, rd 1.075105 sec)
  7 (wr 0.860736 sec, rd 0.652264 sec)
  8 (wr 0.440386 sec, rd 0.240520 sec)
total average: (wr 1.946450 sec, rd 1.732069 sec)
benched 32.00 GiB worth of memory (292.57 MiB/s) in 111.749234 secs total
```

#### i7-4702MQ
```
discovered Intel(R) Core(TM) i7-4702MQ CPU @ 2.20GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.733916 sec, rd 2.436790 sec)
thread pair averages:
  1 (wr 3.743545 sec, rd 3.474793 sec)
  2 (wr 3.219560 sec, rd 3.001925 sec)
  3 (wr 2.765952 sec, rd 2.529974 sec)
  4 (wr 2.282410 sec, rd 2.048847 sec)
  5 (wr 1.821917 sec, rd 1.577716 sec)
  6 (wr 1.343491 sec, rd 1.115553 sec)
  7 (wr 0.880156 sec, rd 0.664208 sec)
  8 (wr 0.461330 sec, rd 0.219415 sec)
total average: (wr 2.064795 sec, rd 1.829054 sec)
benched 32.00 GiB worth of memory (270.80 MiB/s) in 120.089799 secs total
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
#### i7-4800MQ
```
discovered Intel(R) Core(TM) i7-4800MQ CPU @ 2.70GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.662296 sec, rd 2.386916 sec)
thread pair averages:
  1 (wr 3.648443 sec, rd 3.393188 sec)
  2 (wr 3.167436 sec, rd 2.935026 sec)
  3 (wr 2.709503 sec, rd 2.465347 sec)
  4 (wr 2.224371 sec, rd 1.980122 sec)
  5 (wr 1.746518 sec, rd 1.511296 sec)
  6 (wr 1.275610 sec, rd 1.010919 sec)
  7 (wr 0.803538 sec, rd 0.613126 sec)
  8 (wr 0.406616 sec, rd 0.222207 sec)
total average: (wr 1.997754 sec, rd 1.766404 sec)
benched 32.00 GiB worth of memory (277.69 MiB/s) in 117.104607 secs total
```

#### i7-6700K
```
discovered Intel(R) Core(TM) i7-6700K CPU @ 4.00GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.343671 sec, rd 2.081862 sec)
thread pair averages:
  1 (wr 2.885558 sec, rd 2.671620 sec)
  2 (wr 2.472182 sec, rd 2.277332 sec)
  3 (wr 2.100868 sec, rd 1.912702 sec)
  4 (wr 1.776112 sec, rd 1.621613 sec)
  5 (wr 1.424414 sec, rd 1.272395 sec)
  6 (wr 1.101160 sec, rd 0.911659 sec)
  7 (wr 0.738997 sec, rd 0.553157 sec)
  8 (wr 0.345031 sec, rd 0.164712 sec)
total average: (wr 1.605540 sec, rd 1.423149 sec)
benched 32.00 GiB worth of memory (352.34 MiB/s) in 92.624301 secs total
```

#### i7-7700K
```
discovered Intel(R) Core(TM) i7-7700K CPU @ 4.20GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 1.717903 sec, rd 1.459413 sec)
thread pair averages:
  1 (wr 2.663711 sec, rd 2.468838 sec)
  2 (wr 2.317031 sec, rd 2.167518 sec)
  3 (wr 2.001989 sec, rd 1.796020 sec)
  4 (wr 1.627218 sec, rd 1.458980 sec)
  5 (wr 1.279291 sec, rd 1.114968 sec)
  6 (wr 0.943537 sec, rd 0.781062 sec)
  7 (wr 0.607803 sec, rd 0.435665 sec)
  8 (wr 0.282312 sec, rd 0.134656 sec)
total average: (wr 1.465362 sec, rd 1.294713 sec)
benched 32.00 GiB worth of memory (381.02 MiB/s) in 85.755991 secs total
```

#### i7 860
```
discovered Intel(R) Core(TM) i7 CPU 860 @ 2.80GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 0] [7, 1]
trial 32 of 32 [%100.00] (wr 2.340896 sec, rd 2.094335 sec)
thread pair averages:
  1 (wr 3.027625 sec, rd 2.806598 sec)
  2 (wr 2.607912 sec, rd 2.399488 sec)
  3 (wr 2.225947 sec, rd 2.068080 sec)
  4 (wr 1.896345 sec, rd 1.697492 sec)
  5 (wr 1.493139 sec, rd 1.312024 sec)
  6 (wr 1.123178 sec, rd 0.906370 sec)
  7 (wr 0.728655 sec, rd 0.579566 sec)
  8 (wr 0.362162 sec, rd 0.165358 sec)
total average: (wr 1.683121 sec, rd 1.491872 sec)
benched 32.00 GiB worth of memory (334.36 MiB/s) in 97.245199 secs total
```

#### Xeon E5-2609
```
discovered Intel(R) Xeon(R) CPU E5-2609 0 @ 2.40GHz: 8 logical CPU(s), 8 physical, 1 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread pair(s) or 16 thread(s) total per trial run
thread cpu pairs: [0, 1] [1, 2] [2, 3] [3, 4] [4, 5] [5, 6] [6, 7] [7, 0]
trial 32 of 32 [%100.00] (wr 3.457064 sec, rd 3.086780 sec)
thread pair averages:
  1 (wr 3.622806 sec, rd 3.382127 sec)
  2 (wr 3.164791 sec, rd 2.931709 sec)
  3 (wr 2.694972 sec, rd 2.470696 sec)
  4 (wr 2.232260 sec, rd 2.017634 sec)
  5 (wr 1.808695 sec, rd 1.585226 sec)
  6 (wr 1.360359 sec, rd 1.116778 sec)
  7 (wr 0.904735 sec, rd 0.669876 sec)
  8 (wr 0.417126 sec, rd 0.198439 sec)
total average: (wr 2.025718 sec, rd 1.796561 sec)
benched 32.00 GiB worth of memory (280.06 MiB/s) in 116.308600 secs total
```

### 12 Thread Configurations

#### Xeon E5-2620
```
discovered Intel(R) Xeon(R) CPU E5-2620 v3 @ 2.40GHz: 12 logical CPU(s), 6 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 12 thread pair(s) or 24 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 0] [11, 1]
trial 32 of 32 [%100.00] (wr 3.527505 sec, rd 3.287416 sec)
thread pair averages:
  1 (wr 4.624307 sec, rd 4.385098 sec)
  2 (wr 4.148821 sec, rd 3.946107 sec)
  3 (wr 3.735245 sec, rd 3.557979 sec)
  4 (wr 3.391874 sec, rd 3.215288 sec)
  5 (wr 3.049918 sec, rd 2.859165 sec)
  6 (wr 2.692640 sec, rd 2.509668 sec)
  7 (wr 2.335570 sec, rd 2.162963 sec)
  8 (wr 1.969208 sec, rd 1.783596 sec)
  9 (wr 1.616843 sec, rd 1.427714 sec)
  10 (wr 1.223167 sec, rd 1.028591 sec)
  11 (wr 0.831462 sec, rd 0.629659 sec)
  12 (wr 0.422261 sec, rd 0.206010 sec)
total average: (wr 2.503443 sec, rd 2.309320 sec)
benched 48.00 GiB worth of memory (329.87 MiB/s) in 148.249378 secs total
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

#### Ryzen 1800x
```
discovered AMD Ryzen 7 1800X Eight-Core Processor: 16 logical CPU(s), 8 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 16 thread pair(s) or 32 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 0] [15, 1]
trial 32 of 32 [%100.00] (wr 3.065228 sec, rd 2.848212 sec)
thread pair averages:
  1 (wr 5.310936 sec, rd 5.111738 sec)
  2 (wr 4.932744 sec, rd 4.742017 sec)
  3 (wr 4.567351 sec, rd 4.388152 sec)
  4 (wr 4.228527 sec, rd 4.056428 sec)
  5 (wr 3.885407 sec, rd 3.696289 sec)
  6 (wr 3.532067 sec, rd 3.370483 sec)
  7 (wr 3.181150 sec, rd 3.014855 sec)
  8 (wr 2.858295 sec, rd 2.660739 sec)
  9 (wr 2.476888 sec, rd 2.296587 sec)
  10 (wr 2.130544 sec, rd 1.974210 sec)
  11 (wr 1.817833 sec, rd 1.683757 sec)
  12 (wr 1.513205 sec, rd 1.361369 sec)
  13 (wr 1.204070 sec, rd 1.049316 sec)
  14 (wr 0.894551 sec, rd 0.740290 sec)
  15 (wr 0.593564 sec, rd 0.444251 sec)
  16 (wr 0.299039 sec, rd 0.148602 sec)
total average: (wr 2.714136 sec, rd 2.546193 sec)
benched 64.00 GiB worth of memory (383.25 MiB/s) in 170.163139 secs total
```

#### Xeon X5672
```
discovered Intel(R) Xeon(R) CPU X5672 @ 3.20GHz: 16 logical CPU(s), 8 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 16 thread pair(s) or 32 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 0] [15, 1]
trial 32 of 32 [%100.00] (wr 1.682639 sec, rd 1.561880 sec)
thread pair averages:
  1 (wr 2.987680 sec, rd 2.861652 sec)
  2 (wr 2.735562 sec, rd 2.615764 sec)
  3 (wr 2.505951 sec, rd 2.392840 sec)
  4 (wr 2.285858 sec, rd 2.181741 sec)
  5 (wr 2.080623 sec, rd 1.981860 sec)
  6 (wr 1.890865 sec, rd 1.799763 sec)
  7 (wr 1.710607 sec, rd 1.614714 sec)
  8 (wr 1.521599 sec, rd 1.432799 sec)
  9 (wr 1.344112 sec, rd 1.253933 sec)
  10 (wr 1.167470 sec, rd 1.091893 sec)
  11 (wr 1.010997 sec, rd 0.935125 sec)
  12 (wr 0.862420 sec, rd 0.780289 sec)
  13 (wr 0.702573 sec, rd 0.616691 sec)
  14 (wr 0.542758 sec, rd 0.454217 sec)
  15 (wr 0.367294 sec, rd 0.286346 sec)
  16 (wr 0.194691 sec, rd 0.102320 sec)
total average: (wr 1.494441 sec, rd 1.400122 sec)
benched 64.00 GiB worth of memory (682.66 MiB/s) in 95.917353 secs total
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

### 64 Thread Configurations

#### Opteron 6376 (4-socket Piledriver)
```
discovered AMD Opteron(tm) Processor 6376: 64 logical CPU(s), 32 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 64 thread pair(s) or 128 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 16] [15, 17] [16, 18] [17, 19] [18, 20] [19, 21] [20, 22] [21, 23] [22, 24] [23, 25] [24, 26] [25, 27] [26, 28] [27, 29] [28, 30] [29, 31] [30, 32] [31, 33] [32, 34] [33, 35] [34, 36] [35, 37] [36, 38] [37, 39] [38, 40] [39, 41] [40, 42] [41, 43] [42, 44] [43, 45] [44, 46] [45, 47] [46, 48] [47, 49] [48, 50] [49, 51] [50, 52] [51, 53] [52, 54] [53, 55] [54, 56] [55, 57] [56, 58] [57, 59] [58, 60] [59, 61] [60, 62] [61, 63] [62, 0] [63, 1]
trial 32 of 32 [%100.00] (wr 7.557485 sec, rd 7.434674 sec)
thread pair averages:
  1 (wr 14.287706 sec, rd 14.140554 sec)
  2 (wr 14.006542 sec, rd 13.868550 sec)
  3 (wr 13.728022 sec, rd 13.579897 sec)
  4 (wr 13.441372 sec, rd 13.305411 sec)
  5 (wr 13.160390 sec, rd 13.008907 sec)
  6 (wr 12.858093 sec, rd 12.722882 sec)
  7 (wr 12.588092 sec, rd 12.436470 sec)
  8 (wr 12.299672 sec, rd 12.170665 sec)
  9 (wr 12.029301 sec, rd 11.887951 sec)
  10 (wr 11.738783 sec, rd 11.619996 sec)
  11 (wr 11.503454 sec, rd 11.383534 sec)
  12 (wr 11.260710 sec, rd 11.137081 sec)
  13 (wr 11.021688 sec, rd 10.899882 sec)
  14 (wr 10.775689 sec, rd 10.653740 sec)
  15 (wr 10.532324 sec, rd 10.421355 sec)
  16 (wr 10.310508 sec, rd 10.198665 sec)
  17 (wr 10.081676 sec, rd 9.970514 sec)
  18 (wr 9.852378 sec, rd 9.741443 sec)
  19 (wr 9.623841 sec, rd 9.512717 sec)
  20 (wr 9.398948 sec, rd 9.284320 sec)
  21 (wr 9.172016 sec, rd 9.057528 sec)
  22 (wr 8.947262 sec, rd 8.821875 sec)
  23 (wr 8.703625 sec, rd 8.583355 sec)
  24 (wr 8.475305 sec, rd 8.367620 sec)
  25 (wr 8.261051 sec, rd 8.144296 sec)
  26 (wr 8.027485 sec, rd 7.915704 sec)
  27 (wr 7.800620 sec, rd 7.688937 sec)
  28 (wr 7.572408 sec, rd 7.456999 sec)
  29 (wr 7.344142 sec, rd 7.228144 sec)
  30 (wr 7.115863 sec, rd 6.999310 sec)
  31 (wr 6.890015 sec, rd 6.781905 sec)
  32 (wr 6.669014 sec, rd 6.566848 sec)
  33 (wr 6.452914 sec, rd 6.338409 sec)
  34 (wr 6.233920 sec, rd 6.131433 sec)
  35 (wr 6.025271 sec, rd 5.922198 sec)
  36 (wr 5.818900 sec, rd 5.714152 sec)
  37 (wr 5.612633 sec, rd 5.513192 sec)
  38 (wr 5.414809 sec, rd 5.305857 sec)
  39 (wr 5.201088 sec, rd 5.106084 sec)
  40 (wr 5.002511 sec, rd 4.907374 sec)
  41 (wr 4.814541 sec, rd 4.712986 sec)
  42 (wr 4.604543 sec, rd 4.506840 sec)
  43 (wr 4.402678 sec, rd 4.307460 sec)
  44 (wr 4.216993 sec, rd 4.116745 sec)
  45 (wr 4.020097 sec, rd 3.920027 sec)
  46 (wr 3.827408 sec, rd 3.723749 sec)
  47 (wr 3.625454 sec, rd 3.524179 sec)
  48 (wr 3.421731 sec, rd 3.322221 sec)
  49 (wr 3.226903 sec, rd 3.137906 sec)
  50 (wr 3.034146 sec, rd 2.938738 sec)
  51 (wr 2.830743 sec, rd 2.732446 sec)
  52 (wr 2.627221 sec, rd 2.525161 sec)
  53 (wr 2.423875 sec, rd 2.319220 sec)
  54 (wr 2.224853 sec, rd 2.128407 sec)
  55 (wr 2.027105 sec, rd 1.924540 sec)
  56 (wr 1.823827 sec, rd 1.724318 sec)
  57 (wr 1.621321 sec, rd 1.520377 sec)
  58 (wr 1.420267 sec, rd 1.321519 sec)
  59 (wr 1.221708 sec, rd 1.117059 sec)
  60 (wr 1.020714 sec, rd 0.925564 sec)
  61 (wr 0.824271 sec, rd 0.718135 sec)
  62 (wr 0.617136 sec, rd 0.510475 sec)
  63 (wr 0.412267 sec, rd 0.302723 sec)
  64 (wr 0.206757 sec, rd 0.108640 sec)
total average: (wr 6.808384 sec, rd 6.696644 sec)
benched 256.00 GiB worth of memory (572.36 MiB/s) in 457.506831 secs total
```

#### Xeon E7-4830
```
discovered Intel(R) Xeon(R) CPU E7- 4830 @ 2.13GHz: 64 logical CPU(s), 32 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 64 thread pair(s) or 128 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 16] [15, 17] [16, 18] [17, 19] [18, 20] [19, 21] [20, 22] [21, 23] [22, 24] [23, 25] [24, 26] [25, 27] [26, 28] [27, 29] [28, 30] [29, 31] [30, 32] [31, 33] [32, 34] [33, 35] [34, 36] [35, 37] [36, 38] [37, 39] [38, 40] [39, 41] [40, 42] [41, 43] [42, 44] [43, 45] [44, 46] [45, 47] [46, 48] [47, 49] [48, 50] [49, 51] [50, 52] [51, 53] [52, 54] [53, 55] [54, 56] [55, 57] [56, 58] [57, 59] [58, 60] [59, 61] [60, 62] [61, 63] [62, 0] [63, 1]
trial 32 of 32 [%100.00] (wr 3.086694 sec, rd 3.035561 sec)
thread pair averages:
  1 (wr 5.847300 sec, rd 5.796392 sec)
  2 (wr 5.752731 sec, rd 5.707243 sec)
  3 (wr 5.662065 sec, rd 5.616072 sec)
  4 (wr 5.566092 sec, rd 5.513297 sec)
  5 (wr 5.462317 sec, rd 5.411158 sec)
  6 (wr 5.356993 sec, rd 5.301049 sec)
  7 (wr 5.249607 sec, rd 5.200304 sec)
  8 (wr 5.153736 sec, rd 5.101995 sec)
  9 (wr 5.049331 sec, rd 4.999260 sec)
  10 (wr 4.949767 sec, rd 4.898881 sec)
  11 (wr 4.847393 sec, rd 4.792652 sec)
  12 (wr 4.742982 sec, rd 4.691786 sec)
  13 (wr 4.643889 sec, rd 4.588940 sec)
  14 (wr 4.537009 sec, rd 4.481914 sec)
  15 (wr 4.428069 sec, rd 4.375354 sec)
  16 (wr 4.315593 sec, rd 4.256122 sec)
  17 (wr 4.196429 sec, rd 4.134273 sec)
  18 (wr 4.072760 sec, rd 4.007763 sec)
  19 (wr 3.942628 sec, rd 3.875766 sec)
  20 (wr 3.811894 sec, rd 3.741761 sec)
  21 (wr 3.672510 sec, rd 3.603020 sec)
  22 (wr 3.540426 sec, rd 3.480910 sec)
  23 (wr 3.419920 sec, rd 3.358738 sec)
  24 (wr 3.293216 sec, rd 3.235517 sec)
  25 (wr 3.177170 sec, rd 3.117184 sec)
  26 (wr 3.059565 sec, rd 3.001806 sec)
  27 (wr 2.945438 sec, rd 2.890612 sec)
  28 (wr 2.832468 sec, rd 2.778210 sec)
  29 (wr 2.722232 sec, rd 2.661921 sec)
  30 (wr 2.602844 sec, rd 2.551239 sec)
  31 (wr 2.497224 sec, rd 2.439991 sec)
  32 (wr 2.380829 sec, rd 2.326858 sec)
  33 (wr 2.275061 sec, rd 2.221941 sec)
  34 (wr 2.171418 sec, rd 2.115612 sec)
  35 (wr 2.063980 sec, rd 2.013531 sec)
  36 (wr 1.968744 sec, rd 1.922478 sec)
  37 (wr 1.888573 sec, rd 1.848487 sec)
  38 (wr 1.807197 sec, rd 1.771384 sec)
  39 (wr 1.727681 sec, rd 1.687657 sec)
  40 (wr 1.647651 sec, rd 1.605835 sec)
  41 (wr 1.571360 sec, rd 1.531390 sec)
  42 (wr 1.491923 sec, rd 1.446591 sec)
  43 (wr 1.407253 sec, rd 1.369061 sec)
  44 (wr 1.326641 sec, rd 1.288176 sec)
  45 (wr 1.248393 sec, rd 1.211295 sec)
  46 (wr 1.175931 sec, rd 1.148982 sec)
  47 (wr 1.123128 sec, rd 1.094720 sec)
  48 (wr 1.063138 sec, rd 1.032190 sec)
  49 (wr 1.004235 sec, rd 0.974416 sec)
  50 (wr 0.946054 sec, rd 0.914361 sec)
  51 (wr 0.883638 sec, rd 0.852239 sec)
  52 (wr 0.820180 sec, rd 0.785301 sec)
  53 (wr 0.755536 sec, rd 0.725438 sec)
  54 (wr 0.695361 sec, rd 0.664083 sec)
  55 (wr 0.635292 sec, rd 0.606028 sec)
  56 (wr 0.576151 sec, rd 0.546959 sec)
  57 (wr 0.516354 sec, rd 0.486138 sec)
  58 (wr 0.454710 sec, rd 0.420553 sec)
  59 (wr 0.385660 sec, rd 0.353287 sec)
  60 (wr 0.320459 sec, rd 0.287894 sec)
  61 (wr 0.256951 sec, rd 0.226716 sec)
  62 (wr 0.197655 sec, rd 0.164426 sec)
  63 (wr 0.133803 sec, rd 0.104321 sec)
  64 (wr 0.074696 sec, rd 0.042441 sec)
total average: (wr 2.630426 sec, rd 2.584405 sec)
benched 256.00 GiB worth of memory (1.36 GiB/s) in 187.653462 secs total
```

### 256 Thread Configurations

### Xeon Phi 7210 (MCDRAM as cache)
```
discovered Intel(R) Xeon Phi(TM) CPU 7210 @ 1.30GHz: 256 logical CPU(s), 64 physical, 4 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 256 thread pair(s) or 512 thread(s) total per trial run
thread cpu pairs: [0, 4] [1, 5] [2, 6] [3, 7] [4, 8] [5, 9] [6, 10] [7, 11] [8, 12] [9, 13] [10, 14] [11, 15] [12, 16] [13, 17] [14, 18] [15, 19] [16, 20] [17, 21] [18, 22] [19, 23] [20, 24] [21, 25] [22, 26] [23, 27] [24, 28] [25, 29] [26, 30] [27, 31] [28, 32] [29, 33] [30, 34] [31, 35] [32, 36] [33, 37] [34, 38] [35, 39] [36, 40] [37, 41] [38, 42] [39, 43] [40, 44] [41, 45] [42, 46] [43, 47] [44, 48] [45, 49] [46, 50] [47, 51] [48, 52] [49, 53] [50, 54] [51, 55] [52, 56] [53, 57] [54, 58] [55, 59] [56, 60] [57, 61] [58, 62] [59, 63] [60, 64] [61, 65] [62, 66] [63, 67] [64, 68] [65, 69] [66, 70] [67, 71] [68, 72] [69, 73] [70, 74] [71, 75] [72, 76] [73, 77] [74, 78] [75, 79] [76, 80] [77, 81] [78, 82] [79, 83] [80, 84] [81, 85] [82, 86] [83, 87] [84, 88] [85, 89] [86, 90] [87, 91] [88, 92] [89, 93] [90, 94] [91, 95] [92, 96] [93, 97] [94, 98] [95, 99] [96, 100] [97, 101] [98, 102] [99, 103] [100, 104] [101, 105] [102, 106] [103, 107] [104, 108] [105, 109] [106, 110] [107, 111] [108, 112] [109, 113] [110, 114] [111, 115] [112, 116] [113, 117] [114, 118] [115, 119] [116, 120] [117, 121] [118, 122] [119, 123] [120, 124] [121, 125] [122, 126] [123, 127] [124, 128] [125, 129] [126, 130] [127, 131] [128, 132] [129, 133] [130, 134] [131, 135] [132, 136] [133, 137] [134, 138] [135, 139] [136, 140] [137, 141] [138, 142] [139, 143] [140, 144] [141, 145] [142, 146] [143, 147] [144, 148] [145, 149] [146, 150] [147, 151] [148, 152] [149, 153] [150, 154] [151, 155] [152, 156] [153, 157] [154, 158] [155, 159] [156, 160] [157, 161] [158, 162] [159, 163] [160, 164] [161, 165] [162, 166] [163, 167] [164, 168] [165, 169] [166, 170] [167, 171] [168, 172] [169, 173] [170, 174] [171, 175] [172, 176] [173, 177] [174, 178] [175, 179] [176, 180] [177, 181] [178, 182] [179, 183] [180, 184] [181, 185] [182, 186] [183, 187] [184, 188] [185, 189] [186, 190] [187, 191] [188, 192] [189, 193] [190, 194] [191, 195] [192, 196] [193, 197] [194, 198] [195, 199] [196, 200] [197, 201] [198, 202] [199, 203] [200, 204] [201, 205] [202, 206] [203, 207] [204, 208] [205, 209] [206, 210] [207, 211] [208, 212] [209, 213] [210, 214] [211, 215] [212, 216] [213, 217] [214, 218] [215, 219] [216, 220] [217, 221] [218, 222] [219, 223] [220, 224] [221, 225] [222, 226] [223, 227] [224, 228] [225, 229] [226, 230] [227, 231] [228, 232] [229, 233] [230, 234] [231, 235] [232, 236] [233, 237] [234, 238] [235, 239] [236, 240] [237, 241] [238, 242] [239, 243] [240, 244] [241, 245] [242, 246] [243, 247] [244, 248] [245, 249] [246, 250] [247, 251] [248, 252] [249, 253] [250, 254] [251, 255] [252, 0] [253, 1] [254, 2] [255, 3]
trial 32 of 32 [%100.00] (wr 5.063650 sec, rd 5.030199 sec)
thread pair averages:
  1 (wr 16.267116 sec, rd 16.202236 sec)
  2 (wr 16.147396 sec, rd 16.093721 sec)
  3 (wr 16.040986 sec, rd 15.988246 sec)
  4 (wr 15.938559 sec, rd 15.884863 sec)
  5 (wr 15.831470 sec, rd 15.776326 sec)
  6 (wr 15.725625 sec, rd 15.673076 sec)
  7 (wr 15.620365 sec, rd 15.565530 sec)
  8 (wr 15.514002 sec, rd 15.460873 sec)
  9 (wr 15.408672 sec, rd 15.356013 sec)
  10 (wr 15.301237 sec, rd 15.248285 sec)
  11 (wr 15.196328 sec, rd 15.144032 sec)
  12 (wr 15.089042 sec, rd 15.037334 sec)
  13 (wr 14.981375 sec, rd 14.928866 sec)
  14 (wr 14.878490 sec, rd 14.824415 sec)
  15 (wr 14.768635 sec, rd 14.717512 sec)
  16 (wr 14.664883 sec, rd 14.611265 sec)
  17 (wr 14.557935 sec, rd 14.503981 sec)
  18 (wr 14.447794 sec, rd 14.396526 sec)
  19 (wr 14.339622 sec, rd 14.287716 sec)
  20 (wr 14.236411 sec, rd 14.183394 sec)
  21 (wr 14.127331 sec, rd 14.072484 sec)
  22 (wr 14.017582 sec, rd 13.963810 sec)
  23 (wr 13.911398 sec, rd 13.857849 sec)
  24 (wr 13.799867 sec, rd 13.745894 sec)
  25 (wr 13.691219 sec, rd 13.637593 sec)
  26 (wr 13.582199 sec, rd 13.525965 sec)
  27 (wr 13.473474 sec, rd 13.418284 sec)
  28 (wr 13.363108 sec, rd 13.309137 sec)
  29 (wr 13.254019 sec, rd 13.198895 sec)
  30 (wr 13.147097 sec, rd 13.092840 sec)
  31 (wr 13.040372 sec, rd 12.987763 sec)
  32 (wr 12.935441 sec, rd 12.884239 sec)
  33 (wr 12.828173 sec, rd 12.775775 sec)
  34 (wr 12.719562 sec, rd 12.668339 sec)
  35 (wr 12.615342 sec, rd 12.562523 sec)
  36 (wr 12.513099 sec, rd 12.455417 sec)
  37 (wr 12.402375 sec, rd 12.348862 sec)
  38 (wr 12.294545 sec, rd 12.242498 sec)
  39 (wr 12.189099 sec, rd 12.137284 sec)
  40 (wr 12.084723 sec, rd 12.035247 sec)
  41 (wr 11.984872 sec, rd 11.933588 sec)
  42 (wr 11.877486 sec, rd 11.824340 sec)
  43 (wr 11.770232 sec, rd 11.717177 sec)
  44 (wr 11.663888 sec, rd 11.608475 sec)
  45 (wr 11.557623 sec, rd 11.502683 sec)
  46 (wr 11.447044 sec, rd 11.394817 sec)
  47 (wr 11.340523 sec, rd 11.286030 sec)
  48 (wr 11.232240 sec, rd 11.178580 sec)
  49 (wr 11.122192 sec, rd 11.067075 sec)
  50 (wr 11.013812 sec, rd 10.961815 sec)
  51 (wr 10.909092 sec, rd 10.855763 sec)
  52 (wr 10.799636 sec, rd 10.745440 sec)
  53 (wr 10.691001 sec, rd 10.637394 sec)
  54 (wr 10.584241 sec, rd 10.534272 sec)
  55 (wr 10.481698 sec, rd 10.424908 sec)
  56 (wr 10.373046 sec, rd 10.319943 sec)
  57 (wr 10.266464 sec, rd 10.214161 sec)
  58 (wr 10.159277 sec, rd 10.106743 sec)
  59 (wr 10.052561 sec, rd 9.998268 sec)
  60 (wr 9.944532 sec, rd 9.890054 sec)
  61 (wr 9.834308 sec, rd 9.780351 sec)
  62 (wr 9.726241 sec, rd 9.671947 sec)
  63 (wr 9.617445 sec, rd 9.564346 sec)
  64 (wr 9.510226 sec, rd 9.456001 sec)
  65 (wr 9.404629 sec, rd 9.351730 sec)
  66 (wr 9.299198 sec, rd 9.246765 sec)
  67 (wr 9.196472 sec, rd 9.147054 sec)
  68 (wr 9.096354 sec, rd 9.044054 sec)
  69 (wr 8.989944 sec, rd 8.937194 sec)
  70 (wr 8.882816 sec, rd 8.829045 sec)
  71 (wr 8.774272 sec, rd 8.722495 sec)
  72 (wr 8.670336 sec, rd 8.616166 sec)
  73 (wr 8.567203 sec, rd 8.515187 sec)
  74 (wr 8.460904 sec, rd 8.407063 sec)
  75 (wr 8.352118 sec, rd 8.297584 sec)
  76 (wr 8.246465 sec, rd 8.193131 sec)
  77 (wr 8.138688 sec, rd 8.086741 sec)
  78 (wr 8.033190 sec, rd 7.977714 sec)
  79 (wr 7.924876 sec, rd 7.867899 sec)
  80 (wr 7.816040 sec, rd 7.760998 sec)
  81 (wr 7.708007 sec, rd 7.654976 sec)
  82 (wr 7.599399 sec, rd 7.546717 sec)
  83 (wr 7.490654 sec, rd 7.435725 sec)
  84 (wr 7.381038 sec, rd 7.327546 sec)
  85 (wr 7.272162 sec, rd 7.217154 sec)
  86 (wr 7.165230 sec, rd 7.109425 sec)
  87 (wr 7.055550 sec, rd 6.999119 sec)
  88 (wr 6.944279 sec, rd 6.891203 sec)
  89 (wr 6.838393 sec, rd 6.784852 sec)
  90 (wr 6.730804 sec, rd 6.678256 sec)
  91 (wr 6.624962 sec, rd 6.570698 sec)
  92 (wr 6.514945 sec, rd 6.460812 sec)
  93 (wr 6.405597 sec, rd 6.349714 sec)
  94 (wr 6.294348 sec, rd 6.238299 sec)
  95 (wr 6.187900 sec, rd 6.134529 sec)
  96 (wr 6.081566 sec, rd 6.028149 sec)
  97 (wr 5.975066 sec, rd 5.922598 sec)
  98 (wr 5.871237 sec, rd 5.817198 sec)
  99 (wr 5.761239 sec, rd 5.707282 sec)
  100 (wr 5.654791 sec, rd 5.602024 sec)
  101 (wr 5.553030 sec, rd 5.498200 sec)
  102 (wr 5.444720 sec, rd 5.391786 sec)
  103 (wr 5.336616 sec, rd 5.282333 sec)
  104 (wr 5.221982 sec, rd 5.160567 sec)
  105 (wr 5.097994 sec, rd 5.020205 sec)
  106 (wr 4.933883 sec, rd 4.832740 sec)
  107 (wr 4.683603 sec, rd 4.514306 sec)
  108 (wr 4.316509 sec, rd 4.112840 sec)
  109 (wr 3.880550 sec, rd 3.669872 sec)
  110 (wr 3.485908 sec, rd 3.322657 sec)
  111 (wr 3.186323 sec, rd 3.056194 sec)
  112 (wr 2.978823 sec, rd 2.925694 sec)
  113 (wr 2.886839 sec, rd 2.851416 sec)
  114 (wr 2.820430 sec, rd 2.791110 sec)
  115 (wr 2.766423 sec, rd 2.740757 sec)
  116 (wr 2.716230 sec, rd 2.691359 sec)
  117 (wr 2.667567 sec, rd 2.642120 sec)
  118 (wr 2.615517 sec, rd 2.594811 sec)
  119 (wr 2.572297 sec, rd 2.548798 sec)
  120 (wr 2.524431 sec, rd 2.504327 sec)
  121 (wr 2.481905 sec, rd 2.457878 sec)
  122 (wr 2.438140 sec, rd 2.415233 sec)
  123 (wr 2.393734 sec, rd 2.367842 sec)
  124 (wr 2.344883 sec, rd 2.326757 sec)
  125 (wr 2.300436 sec, rd 2.277078 sec)
  126 (wr 2.251870 sec, rd 2.221935 sec)
  127 (wr 2.197884 sec, rd 2.172587 sec)
  128 (wr 2.148823 sec, rd 2.124987 sec)
  129 (wr 2.101087 sec, rd 2.081155 sec)
  130 (wr 2.059302 sec, rd 2.031932 sec)
  131 (wr 2.008474 sec, rd 1.986503 sec)
  132 (wr 1.961385 sec, rd 1.938271 sec)
  133 (wr 1.913957 sec, rd 1.886794 sec)
  134 (wr 1.863352 sec, rd 1.837757 sec)
  135 (wr 1.813604 sec, rd 1.788067 sec)
  136 (wr 1.761470 sec, rd 1.734204 sec)
  137 (wr 1.710179 sec, rd 1.684882 sec)
  138 (wr 1.662511 sec, rd 1.635108 sec)
  139 (wr 1.607101 sec, rd 1.579277 sec)
  140 (wr 1.555459 sec, rd 1.532566 sec)
  141 (wr 1.504302 sec, rd 1.478653 sec)
  142 (wr 1.453831 sec, rd 1.427317 sec)
  143 (wr 1.403749 sec, rd 1.378744 sec)
  144 (wr 1.356514 sec, rd 1.328319 sec)
  145 (wr 1.306217 sec, rd 1.283554 sec)
  146 (wr 1.259470 sec, rd 1.240139 sec)
  147 (wr 1.218149 sec, rd 1.195240 sec)
  148 (wr 1.166969 sec, rd 1.142419 sec)
  149 (wr 1.120414 sec, rd 1.097231 sec)
  150 (wr 1.075888 sec, rd 1.049262 sec)
  151 (wr 1.026659 sec, rd 1.002151 sec)
  152 (wr 0.978205 sec, rd 0.957861 sec)
  153 (wr 0.934130 sec, rd 0.911548 sec)
  154 (wr 0.888579 sec, rd 0.867316 sec)
  155 (wr 0.848800 sec, rd 0.828251 sec)
  156 (wr 0.810658 sec, rd 0.786338 sec)
  157 (wr 0.761235 sec, rd 0.739658 sec)
  158 (wr 0.719531 sec, rd 0.697600 sec)
  159 (wr 0.675504 sec, rd 0.655192 sec)
  160 (wr 0.633878 sec, rd 0.611204 sec)
  161 (wr 0.588670 sec, rd 0.565539 sec)
  162 (wr 0.547761 sec, rd 0.527325 sec)
  163 (wr 0.509146 sec, rd 0.492915 sec)
  164 (wr 0.473356 sec, rd 0.455551 sec)
  165 (wr 0.440447 sec, rd 0.427137 sec)
  166 (wr 0.407152 sec, rd 0.389677 sec)
  167 (wr 0.375339 sec, rd 0.361341 sec)
  168 (wr 0.346024 sec, rd 0.333576 sec)
  169 (wr 0.321944 sec, rd 0.308681 sec)
  170 (wr 0.293552 sec, rd 0.279134 sec)
  171 (wr 0.270074 sec, rd 0.259499 sec)
  172 (wr 0.247774 sec, rd 0.235038 sec)
  173 (wr 0.227779 sec, rd 0.221147 sec)
  174 (wr 0.213592 sec, rd 0.206714 sec)
  175 (wr 0.198628 sec, rd 0.192362 sec)
  176 (wr 0.186454 sec, rd 0.179817 sec)
  177 (wr 0.174426 sec, rd 0.169082 sec)
  178 (wr 0.165532 sec, rd 0.161933 sec)
  179 (wr 0.156686 sec, rd 0.152674 sec)
  180 (wr 0.149919 sec, rd 0.146792 sec)
  181 (wr 0.144121 sec, rd 0.140907 sec)
  182 (wr 0.138479 sec, rd 0.137294 sec)
  183 (wr 0.134535 sec, rd 0.132600 sec)
  184 (wr 0.130701 sec, rd 0.128692 sec)
  185 (wr 0.127386 sec, rd 0.126319 sec)
  186 (wr 0.124670 sec, rd 0.124144 sec)
  187 (wr 0.123615 sec, rd 0.122822 sec)
  188 (wr 0.122048 sec, rd 0.121283 sec)
  189 (wr 0.120501 sec, rd 0.120054 sec)
  190 (wr 0.119632 sec, rd 0.118862 sec)
  191 (wr 0.118383 sec, rd 0.117888 sec)
  192 (wr 0.117437 sec, rd 0.116960 sec)
  193 (wr 0.116481 sec, rd 0.116003 sec)
  194 (wr 0.115536 sec, rd 0.115058 sec)
  195 (wr 0.114590 sec, rd 0.114119 sec)
  196 (wr 0.113647 sec, rd 0.113170 sec)
  197 (wr 0.112696 sec, rd 0.112225 sec)
  198 (wr 0.111744 sec, rd 0.111251 sec)
  199 (wr 0.110779 sec, rd 0.110315 sec)
  200 (wr 0.109836 sec, rd 0.109351 sec)
  201 (wr 0.108878 sec, rd 0.108412 sec)
  202 (wr 0.107943 sec, rd 0.107476 sec)
  203 (wr 0.107010 sec, rd 0.106548 sec)
  204 (wr 0.106086 sec, rd 0.105627 sec)
  205 (wr 0.105168 sec, rd 0.104711 sec)
  206 (wr 0.104233 sec, rd 0.103747 sec)
  207 (wr 0.103265 sec, rd 0.102797 sec)
  208 (wr 0.102332 sec, rd 0.101860 sec)
  209 (wr 0.101340 sec, rd 0.100884 sec)
  210 (wr 0.100435 sec, rd 0.099974 sec)
  211 (wr 0.099513 sec, rd 0.099050 sec)
  212 (wr 0.098591 sec, rd 0.098136 sec)
  213 (wr 0.097680 sec, rd 0.097244 sec)
  214 (wr 0.096799 sec, rd 0.096353 sec)
  215 (wr 0.095903 sec, rd 0.095459 sec)
  216 (wr 0.095017 sec, rd 0.094571 sec)
  217 (wr 0.094130 sec, rd 0.093695 sec)
  218 (wr 0.093268 sec, rd 0.092819 sec)
  219 (wr 0.092376 sec, rd 0.091947 sec)
  220 (wr 0.091517 sec, rd 0.091079 sec)
  221 (wr 0.090651 sec, rd 0.090227 sec)
  222 (wr 0.089796 sec, rd 0.089298 sec)
  223 (wr 0.088836 sec, rd 0.088340 sec)
  224 (wr 0.087853 sec, rd 0.087354 sec)
  225 (wr 0.086852 sec, rd 0.086362 sec)
  226 (wr 0.085852 sec, rd 0.085350 sec)
  227 (wr 0.084826 sec, rd 0.084287 sec)
  228 (wr 0.083806 sec, rd 0.083278 sec)
  229 (wr 0.082779 sec, rd 0.082294 sec)
  230 (wr 0.081794 sec, rd 0.081285 sec)
  231 (wr 0.080784 sec, rd 0.080311 sec)
  232 (wr 0.079846 sec, rd 0.079386 sec)
  233 (wr 0.078917 sec, rd 0.078459 sec)
  234 (wr 0.077980 sec, rd 0.077495 sec)
  235 (wr 0.077035 sec, rd 0.076606 sec)
  236 (wr 0.076164 sec, rd 0.075719 sec)
  237 (wr 0.075297 sec, rd 0.074869 sec)
  238 (wr 0.074399 sec, rd 0.073955 sec)
  239 (wr 0.073524 sec, rd 0.073094 sec)
  240 (wr 0.072631 sec, rd 0.072198 sec)
  241 (wr 0.071733 sec, rd 0.071264 sec)
  242 (wr 0.070787 sec, rd 0.070303 sec)
  243 (wr 0.069820 sec, rd 0.069368 sec)
  244 (wr 0.068909 sec, rd 0.068461 sec)
  245 (wr 0.068017 sec, rd 0.067582 sec)
  246 (wr 0.067159 sec, rd 0.066744 sec)
  247 (wr 0.066249 sec, rd 0.065706 sec)
  248 (wr 0.065221 sec, rd 0.064732 sec)
  249 (wr 0.064264 sec, rd 0.063806 sec)
  250 (wr 0.063323 sec, rd 0.062862 sec)
  251 (wr 0.062428 sec, rd 0.061999 sec)
  252 (wr 0.061586 sec, rd 0.061159 sec)
  253 (wr 0.060708 sec, rd 0.060258 sec)
  254 (wr 0.059827 sec, rd 0.058890 sec)
  255 (wr 0.058420 sec, rd 0.057954 sec)
  256 (wr 0.057523 sec, rd 0.057072 sec)
total average: (wr 4.871533 sec, rd 4.839778 sec)
benched 1.00 TiB worth of memory (1.96 GiB/s) in 521.263057 secs total
```

### Xeon Phi 7210
```
discovered Intel(R) Xeon Phi(TM) CPU 7210 @ 1.30GHz: 256 logical CPU(s), 64 physical, 4 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 256 thread pair(s) or 512 thread(s) total per trial run
thread cpu pairs: [0, 4] [1, 5] [2, 6] [3, 7] [4, 8] [5, 9] [6, 10] [7, 11] [8, 12] [9, 13] [10, 14] [11, 15] [12, 16] [13, 17] [14, 18] [15, 19] [16, 20] [17, 21] [18, 22] [19, 23] [20, 24] [21, 25] [22, 26] [23, 27] [24, 28] [25, 29] [26, 30] [27, 31] [28, 32] [29, 33] [30, 34] [31, 35] [32, 36] [33, 37] [34, 38] [35, 39] [36, 40] [37, 41] [38, 42] [39, 43] [40, 44] [41, 45] [42, 46] [43, 47] [44, 48] [45, 49] [46, 50] [47, 51] [48, 52] [49, 53] [50, 54] [51, 55] [52, 56] [53, 57] [54, 58] [55, 59] [56, 60] [57, 61] [58, 62] [59, 63] [60, 64] [61, 65] [62, 66] [63, 67] [64, 68] [65, 69] [66, 70] [67, 71] [68, 72] [69, 73] [70, 74] [71, 75] [72, 76] [73, 77] [74, 78] [75, 79] [76, 80] [77, 81] [78, 82] [79, 83] [80, 84] [81, 85] [82, 86] [83, 87] [84, 88] [85, 89] [86, 90] [87, 91] [88, 92] [89, 93] [90, 94] [91, 95] [92, 96] [93, 97] [94, 98] [95, 99] [96, 100] [97, 101] [98, 102] [99, 103] [100, 104] [101, 105] [102, 106] [103, 107] [104, 108] [105, 109] [106, 110] [107, 111] [108, 112] [109, 113] [110, 114] [111, 115] [112, 116] [113, 117] [114, 118] [115, 119] [116, 120] [117, 121] [118, 122] [119, 123] [120, 124] [121, 125] [122, 126] [123, 127] [124, 128] [125, 129] [126, 130] [127, 131] [128, 132] [129, 133] [130, 134] [131, 135] [132, 136] [133, 137] [134, 138] [135, 139] [136, 140] [137, 141] [138, 142] [139, 143] [140, 144] [141, 145] [142, 146] [143, 147] [144, 148] [145, 149] [146, 150] [147, 151] [148, 152] [149, 153] [150, 154] [151, 155] [152, 156] [153, 157] [154, 158] [155, 159] [156, 160] [157, 161] [158, 162] [159, 163] [160, 164] [161, 165] [162, 166] [163, 167] [164, 168] [165, 169] [166, 170] [167, 171] [168, 172] [169, 173] [170, 174] [171, 175] [172, 176] [173, 177] [174, 178] [175, 179] [176, 180] [177, 181] [178, 182] [179, 183] [180, 184] [181, 185] [182, 186] [183, 187] [184, 188] [185, 189] [186, 190] [187, 191] [188, 192] [189, 193] [190, 194] [191, 195] [192, 196] [193, 197] [194, 198] [195, 199] [196, 200] [197, 201] [198, 202] [199, 203] [200, 204] [201, 205] [202, 206] [203, 207] [204, 208] [205, 209] [206, 210] [207, 211] [208, 212] [209, 213] [210, 214] [211, 215] [212, 216] [213, 217] [214, 218] [215, 219] [216, 220] [217, 221] [218, 222] [219, 223] [220, 224] [221, 225] [222, 226] [223, 227] [224, 228] [225, 229] [226, 230] [227, 231] [228, 232] [229, 233] [230, 234] [231, 235] [232, 236] [233, 237] [234, 238] [235, 239] [236, 240] [237, 241] [238, 242] [239, 243] [240, 244] [241, 245] [242, 246] [243, 247] [244, 248] [245, 249] [246, 250] [247, 251] [248, 252] [249, 253] [250, 254] [251, 255] [252, 0] [253, 1] [254, 2] [255, 3]
trial 32 of 32 [%100.00] (wr 5.308163 sec, rd 5.275476 sec)
thread pair averages:
  1 (wr 16.837269 sec, rd 16.770611 sec)
  2 (wr 16.716191 sec, rd 16.664601 sec)
  3 (wr 16.611801 sec, rd 16.557204 sec)
  4 (wr 16.504364 sec, rd 16.452790 sec)
  5 (wr 16.401032 sec, rd 16.346629 sec)
  6 (wr 16.293541 sec, rd 16.238623 sec)
  7 (wr 16.186308 sec, rd 16.130868 sec)
  8 (wr 16.078080 sec, rd 16.023688 sec)
  9 (wr 15.970490 sec, rd 15.916759 sec)
  10 (wr 15.864874 sec, rd 15.809822 sec)
  11 (wr 15.755349 sec, rd 15.703427 sec)
  12 (wr 15.650050 sec, rd 15.597206 sec)
  13 (wr 15.543009 sec, rd 15.487981 sec)
  14 (wr 15.436195 sec, rd 15.385245 sec)
  15 (wr 15.330520 sec, rd 15.276693 sec)
  16 (wr 15.224874 sec, rd 15.169738 sec)
  17 (wr 15.114185 sec, rd 15.060323 sec)
  18 (wr 15.010255 sec, rd 14.956168 sec)
  19 (wr 14.900511 sec, rd 14.846344 sec)
  20 (wr 14.793135 sec, rd 14.738784 sec)
  21 (wr 14.686382 sec, rd 14.634692 sec)
  22 (wr 14.581032 sec, rd 14.529713 sec)
  23 (wr 14.475493 sec, rd 14.420041 sec)
  24 (wr 14.369743 sec, rd 14.317522 sec)
  25 (wr 14.266667 sec, rd 14.213168 sec)
  26 (wr 14.160595 sec, rd 14.107080 sec)
  27 (wr 14.051956 sec, rd 13.996820 sec)
  28 (wr 13.942655 sec, rd 13.890608 sec)
  29 (wr 13.837308 sec, rd 13.783461 sec)
  30 (wr 13.730532 sec, rd 13.677095 sec)
  31 (wr 13.624795 sec, rd 13.569481 sec)
  32 (wr 13.515363 sec, rd 13.461575 sec)
  33 (wr 13.407340 sec, rd 13.352735 sec)
  34 (wr 13.299017 sec, rd 13.242322 sec)
  35 (wr 13.190922 sec, rd 13.136266 sec)
  36 (wr 13.080735 sec, rd 13.024771 sec)
  37 (wr 12.970892 sec, rd 12.917647 sec)
  38 (wr 12.869537 sec, rd 12.816201 sec)
  39 (wr 12.763976 sec, rd 12.713443 sec)
  40 (wr 12.659056 sec, rd 12.605719 sec)
  41 (wr 12.553724 sec, rd 12.499712 sec)
  42 (wr 12.448211 sec, rd 12.396061 sec)
  43 (wr 12.341117 sec, rd 12.288807 sec)
  44 (wr 12.235442 sec, rd 12.185418 sec)
  45 (wr 12.131070 sec, rd 12.079608 sec)
  46 (wr 12.029546 sec, rd 11.973189 sec)
  47 (wr 11.921329 sec, rd 11.866680 sec)
  48 (wr 11.811745 sec, rd 11.763312 sec)
  49 (wr 11.708170 sec, rd 11.655741 sec)
  50 (wr 11.604227 sec, rd 11.548372 sec)
  51 (wr 11.493616 sec, rd 11.440859 sec)
  52 (wr 11.386734 sec, rd 11.330496 sec)
  53 (wr 11.277215 sec, rd 11.220495 sec)
  54 (wr 11.165703 sec, rd 11.109983 sec)
  55 (wr 11.058277 sec, rd 11.004033 sec)
  56 (wr 10.950734 sec, rd 10.895414 sec)
  57 (wr 10.841367 sec, rd 10.789502 sec)
  58 (wr 10.737570 sec, rd 10.684310 sec)
  59 (wr 10.633087 sec, rd 10.577836 sec)
  60 (wr 10.521670 sec, rd 10.469267 sec)
  61 (wr 10.416256 sec, rd 10.363764 sec)
  62 (wr 10.312158 sec, rd 10.258992 sec)
  63 (wr 10.204983 sec, rd 10.152173 sec)
  64 (wr 10.096887 sec, rd 10.044365 sec)
  65 (wr 9.989803 sec, rd 9.939947 sec)
  66 (wr 9.886483 sec, rd 9.833344 sec)
  67 (wr 9.782342 sec, rd 9.725836 sec)
  68 (wr 9.672799 sec, rd 9.618954 sec)
  69 (wr 9.561359 sec, rd 9.510186 sec)
  70 (wr 9.455775 sec, rd 9.404708 sec)
  71 (wr 9.351689 sec, rd 9.297156 sec)
  72 (wr 9.244961 sec, rd 9.190269 sec)
  73 (wr 9.137053 sec, rd 9.084960 sec)
  74 (wr 9.030365 sec, rd 8.974881 sec)
  75 (wr 8.920735 sec, rd 8.867478 sec)
  76 (wr 8.812921 sec, rd 8.757945 sec)
  77 (wr 8.702476 sec, rd 8.647818 sec)
  78 (wr 8.594493 sec, rd 8.540220 sec)
  79 (wr 8.484180 sec, rd 8.431398 sec)
  80 (wr 8.376364 sec, rd 8.325723 sec)
  81 (wr 8.272183 sec, rd 8.215691 sec)
  82 (wr 8.161354 sec, rd 8.108730 sec)
  83 (wr 8.056749 sec, rd 8.001825 sec)
  84 (wr 7.949948 sec, rd 7.896711 sec)
  85 (wr 7.844808 sec, rd 7.791454 sec)
  86 (wr 7.738570 sec, rd 7.683855 sec)
  87 (wr 7.630638 sec, rd 7.577794 sec)
  88 (wr 7.527926 sec, rd 7.474448 sec)
  89 (wr 7.421080 sec, rd 7.367553 sec)
  90 (wr 7.313872 sec, rd 7.260311 sec)
  91 (wr 7.209003 sec, rd 7.153644 sec)
  92 (wr 7.100076 sec, rd 7.047040 sec)
  93 (wr 6.992906 sec, rd 6.939763 sec)
  94 (wr 6.885610 sec, rd 6.830467 sec)
  95 (wr 6.776915 sec, rd 6.725029 sec)
  96 (wr 6.674052 sec, rd 6.619081 sec)
  97 (wr 6.563682 sec, rd 6.509021 sec)
  98 (wr 6.453339 sec, rd 6.400939 sec)
  99 (wr 6.344670 sec, rd 6.292395 sec)
  100 (wr 6.237248 sec, rd 6.182694 sec)
  101 (wr 6.127302 sec, rd 6.074962 sec)
  102 (wr 6.023671 sec, rd 5.968500 sec)
  103 (wr 5.914570 sec, rd 5.858861 sec)
  104 (wr 5.803789 sec, rd 5.748747 sec)
  105 (wr 5.686736 sec, rd 5.616549 sec)
  106 (wr 5.539833 sec, rd 5.449274 sec)
  107 (wr 5.335750 sec, rd 5.215768 sec)
  108 (wr 5.021201 sec, rd 4.818607 sec)
  109 (wr 4.579807 sec, rd 4.315767 sec)
  110 (wr 4.046246 sec, rd 3.828306 sec)
  111 (wr 3.638328 sec, rd 3.499306 sec)
  112 (wr 3.393949 sec, rd 3.295112 sec)
  113 (wr 3.233402 sec, rd 3.187353 sec)
  114 (wr 3.083831 sec, rd 3.055426 sec)
  115 (wr 3.028405 sec, rd 3.006076 sec)
  116 (wr 2.981933 sec, rd 2.958163 sec)
  117 (wr 2.935450 sec, rd 2.910777 sec)
  118 (wr 2.885839 sec, rd 2.862354 sec)
  119 (wr 2.838305 sec, rd 2.815048 sec)
  120 (wr 2.790243 sec, rd 2.768189 sec)
  121 (wr 2.745055 sec, rd 2.723081 sec)
  122 (wr 2.695917 sec, rd 2.672512 sec)
  123 (wr 2.645462 sec, rd 2.619079 sec)
  124 (wr 2.597736 sec, rd 2.571437 sec)
  125 (wr 2.545295 sec, rd 2.522856 sec)
  126 (wr 2.497929 sec, rd 2.471836 sec)
  127 (wr 2.448418 sec, rd 2.422423 sec)
  128 (wr 2.392803 sec, rd 2.366310 sec)
  129 (wr 2.339097 sec, rd 2.313196 sec)
  130 (wr 2.284928 sec, rd 2.260250 sec)
  131 (wr 2.233269 sec, rd 2.203871 sec)
  132 (wr 2.175741 sec, rd 2.146844 sec)
  133 (wr 2.117688 sec, rd 2.091710 sec)
  134 (wr 2.067107 sec, rd 2.036698 sec)
  135 (wr 2.004448 sec, rd 1.975922 sec)
  136 (wr 1.944071 sec, rd 1.916602 sec)
  137 (wr 1.889714 sec, rd 1.861888 sec)
  138 (wr 1.834023 sec, rd 1.805974 sec)
  139 (wr 1.775682 sec, rd 1.748760 sec)
  140 (wr 1.719702 sec, rd 1.693050 sec)
  141 (wr 1.661393 sec, rd 1.636767 sec)
  142 (wr 1.616012 sec, rd 1.590733 sec)
  143 (wr 1.560158 sec, rd 1.530103 sec)
  144 (wr 1.508129 sec, rd 1.482306 sec)
  145 (wr 1.461122 sec, rd 1.435509 sec)
  146 (wr 1.409913 sec, rd 1.385899 sec)
  147 (wr 1.364428 sec, rd 1.338551 sec)
  148 (wr 1.314604 sec, rd 1.286575 sec)
  149 (wr 1.261365 sec, rd 1.236282 sec)
  150 (wr 1.210512 sec, rd 1.183854 sec)
  151 (wr 1.167809 sec, rd 1.140641 sec)
  152 (wr 1.119209 sec, rd 1.094495 sec)
  153 (wr 1.066278 sec, rd 1.042537 sec)
  154 (wr 1.018572 sec, rd 0.996325 sec)
  155 (wr 0.974876 sec, rd 0.952364 sec)
  156 (wr 0.932669 sec, rd 0.905589 sec)
  157 (wr 0.880353 sec, rd 0.857758 sec)
  158 (wr 0.835026 sec, rd 0.810185 sec)
  159 (wr 0.788665 sec, rd 0.762795 sec)
  160 (wr 0.738940 sec, rd 0.708471 sec)
  161 (wr 0.679128 sec, rd 0.656339 sec)
  162 (wr 0.635108 sec, rd 0.620535 sec)
  163 (wr 0.597116 sec, rd 0.578993 sec)
  164 (wr 0.560565 sec, rd 0.540922 sec)
  165 (wr 0.522139 sec, rd 0.506178 sec)
  166 (wr 0.487883 sec, rd 0.470592 sec)
  167 (wr 0.456677 sec, rd 0.439559 sec)
  168 (wr 0.427465 sec, rd 0.415808 sec)
  169 (wr 0.402986 sec, rd 0.389386 sec)
  170 (wr 0.373914 sec, rd 0.361553 sec)
  171 (wr 0.348314 sec, rd 0.339440 sec)
  172 (wr 0.330005 sec, rd 0.320650 sec)
  173 (wr 0.312649 sec, rd 0.301780 sec)
  174 (wr 0.292363 sec, rd 0.285598 sec)
  175 (wr 0.278591 sec, rd 0.269314 sec)
  176 (wr 0.260011 sec, rd 0.253233 sec)
  177 (wr 0.245322 sec, rd 0.237367 sec)
  178 (wr 0.229229 sec, rd 0.222725 sec)
  179 (wr 0.218253 sec, rd 0.212421 sec)
  180 (wr 0.204844 sec, rd 0.199543 sec)
  181 (wr 0.195762 sec, rd 0.191394 sec)
  182 (wr 0.185826 sec, rd 0.180772 sec)
  183 (wr 0.177226 sec, rd 0.174637 sec)
  184 (wr 0.170015 sec, rd 0.166257 sec)
  185 (wr 0.162707 sec, rd 0.159850 sec)
  186 (wr 0.157834 sec, rd 0.154074 sec)
  187 (wr 0.151667 sec, rd 0.148104 sec)
  188 (wr 0.145139 sec, rd 0.142475 sec)
  189 (wr 0.138616 sec, rd 0.135442 sec)
  190 (wr 0.133043 sec, rd 0.131904 sec)
  191 (wr 0.129872 sec, rd 0.128731 sec)
  192 (wr 0.127295 sec, rd 0.126456 sec)
  193 (wr 0.125297 sec, rd 0.124777 sec)
  194 (wr 0.123368 sec, rd 0.122215 sec)
  195 (wr 0.121354 sec, rd 0.120520 sec)
  196 (wr 0.120009 sec, rd 0.119188 sec)
  197 (wr 0.118059 sec, rd 0.117534 sec)
  198 (wr 0.116736 sec, rd 0.116213 sec)
  199 (wr 0.115377 sec, rd 0.114864 sec)
  200 (wr 0.114334 sec, rd 0.113809 sec)
  201 (wr 0.113289 sec, rd 0.112770 sec)
  202 (wr 0.112255 sec, rd 0.111729 sec)
  203 (wr 0.111202 sec, rd 0.110684 sec)
  204 (wr 0.110161 sec, rd 0.109316 sec)
  205 (wr 0.108794 sec, rd 0.108279 sec)
  206 (wr 0.107758 sec, rd 0.107231 sec)
  207 (wr 0.106711 sec, rd 0.106176 sec)
  208 (wr 0.105652 sec, rd 0.105129 sec)
  209 (wr 0.104607 sec, rd 0.104088 sec)
  210 (wr 0.103561 sec, rd 0.103037 sec)
  211 (wr 0.102509 sec, rd 0.101922 sec)
  212 (wr 0.101386 sec, rd 0.100850 sec)
  213 (wr 0.100311 sec, rd 0.099795 sec)
  214 (wr 0.099271 sec, rd 0.098741 sec)
  215 (wr 0.098206 sec, rd 0.097663 sec)
  216 (wr 0.097144 sec, rd 0.096600 sec)
  217 (wr 0.096071 sec, rd 0.095537 sec)
  218 (wr 0.095014 sec, rd 0.094480 sec)
  219 (wr 0.093944 sec, rd 0.093418 sec)
  220 (wr 0.092850 sec, rd 0.092303 sec)
  221 (wr 0.091726 sec, rd 0.091216 sec)
  222 (wr 0.090690 sec, rd 0.090158 sec)
  223 (wr 0.089605 sec, rd 0.089064 sec)
  224 (wr 0.088533 sec, rd 0.087969 sec)
  225 (wr 0.087407 sec, rd 0.086878 sec)
  226 (wr 0.086356 sec, rd 0.085798 sec)
  227 (wr 0.085263 sec, rd 0.084734 sec)
  228 (wr 0.084219 sec, rd 0.083670 sec)
  229 (wr 0.083139 sec, rd 0.082621 sec)
  230 (wr 0.082083 sec, rd 0.081542 sec)
  231 (wr 0.081010 sec, rd 0.080492 sec)
  232 (wr 0.079969 sec, rd 0.079437 sec)
  233 (wr 0.078912 sec, rd 0.078383 sec)
  234 (wr 0.077853 sec, rd 0.077324 sec)
  235 (wr 0.076795 sec, rd 0.076275 sec)
  236 (wr 0.075746 sec, rd 0.075212 sec)
  237 (wr 0.074679 sec, rd 0.074162 sec)
  238 (wr 0.073632 sec, rd 0.073098 sec)
  239 (wr 0.072566 sec, rd 0.072044 sec)
  240 (wr 0.071512 sec, rd 0.070970 sec)
  241 (wr 0.070434 sec, rd 0.069896 sec)
  242 (wr 0.069371 sec, rd 0.068841 sec)
  243 (wr 0.068306 sec, rd 0.067779 sec)
  244 (wr 0.067245 sec, rd 0.066715 sec)
  245 (wr 0.066185 sec, rd 0.065657 sec)
  246 (wr 0.065127 sec, rd 0.064589 sec)
  247 (wr 0.064055 sec, rd 0.063427 sec)
  248 (wr 0.062895 sec, rd 0.062362 sec)
  249 (wr 0.061822 sec, rd 0.061298 sec)
  250 (wr 0.060742 sec, rd 0.060203 sec)
  251 (wr 0.059655 sec, rd 0.059130 sec)
  252 (wr 0.058572 sec, rd 0.058029 sec)
  253 (wr 0.057466 sec, rd 0.056932 sec)
  254 (wr 0.056402 sec, rd 0.055466 sec)
  255 (wr 0.054939 sec, rd 0.054395 sec)
  256 (wr 0.053885 sec, rd 0.053326 sec)
total average: (wr 5.167420 sec, rd 5.134753 sec)
benched 1.00 TiB worth of memory (1.89 GiB/s) in 539.607547 secs total
```
