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
trial 32 of 32 [%100.00] (wr 2.297033 sec, rd 2.078533 sec)
thread pair averages:
  1 (wr 0.386904 sec, rd 0.358510 sec)
  2 (wr 0.336491 sec, rd 0.309577 sec)
  3 (wr 0.283108 sec, rd 0.257957 sec)
  4 (wr 0.234133 sec, rd 0.212351 sec)
  5 (wr 0.191534 sec, rd 0.167364 sec)
  6 (wr 0.141259 sec, rd 0.117111 sec)
  7 (wr 0.095446 sec, rd 0.072513 sec)
  8 (wr 0.048263 sec, rd 0.023768 sec)
total average: (wr 13.737098 sec, rd 12.153213 sec)
benched 32.00 GiB worth of memory (327.67 MiB/s) in 99.339052 secs total
```

### 16 Thread Configurations

#### Ryzen 1700
```
discovered AMD Ryzen 7 1700 Eight-Core Processor: 16 logical CPU(s), 8 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 16 thread pair(s) or 32 thread(s) total per trial run
thread cpu pairs: [0, 2] [1, 3] [2, 4] [3, 5] [4, 6] [5, 7] [6, 8] [7, 9] [8, 10] [9, 11] [10, 12] [11, 13] [12, 14] [13, 15] [14, 0] [15, 1]
trial 32 of 32 [%100.00] (wr 4.090589 sec, rd 3.867743 sec)
thread pair averages:
  1 (wr 0.399450 sec, rd 0.385597 sec)
  2 (wr 0.372777 sec, rd 0.359633 sec)
  3 (wr 0.345440 sec, rd 0.332201 sec)
  4 (wr 0.320843 sec, rd 0.307949 sec)
  5 (wr 0.295633 sec, rd 0.285405 sec)
  6 (wr 0.273003 sec, rd 0.260389 sec)
  7 (wr 0.248149 sec, rd 0.236520 sec)
  8 (wr 0.223979 sec, rd 0.211403 sec)
  9 (wr 0.198601 sec, rd 0.185856 sec)
  10 (wr 0.172638 sec, rd 0.158770 sec)
  11 (wr 0.146475 sec, rd 0.134606 sec)
  12 (wr 0.121049 sec, rd 0.108149 sec)
  13 (wr 0.097519 sec, rd 0.084999 sec)
  14 (wr 0.071480 sec, rd 0.059889 sec)
  15 (wr 0.048497 sec, rd 0.036880 sec)
  16 (wr 0.024205 sec, rd 0.012264 sec)
total average: (wr 53.755813 sec, rd 50.568159 sec)
benched 64.00 GiB worth of memory (319.68 MiB/s) in 204.765464 secs total
```
