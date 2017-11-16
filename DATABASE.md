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
configuration's time by 8*2 to scale it to compare relative speed.

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

### 2 Thread Configurations

#### Core i5-2520M (Sandy Bridge-3M) DDR3 2G/4G 1333
```
discovered Intel(R) Core(TM) i5-2520M CPU @ 2.50GHz: 4 logical CPU(s), 2 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 2 thread(s) per trial run
trial 32 of 32 [%100.00] (wr 0.684510 sec, rd 0.446811 sec)
thread averages:
  1 (wr 0.067849 sec, rd 0.047412 sec)
  2 (wr 0.036799 sec, rd 0.022853 sec)
total average: (wr 0.209296 sec, rd 0.140530 sec)
benched 8.00 GiB worth of memory (1.60 GiB/s) in 4.344353 secs total
```

### 4 Thread Configurations

#### i7-4710HQ
```
discovered Intel(R) Core(TM) i7-4710HQ CPU @ 2.50GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 4 thread(s) per trial run
trial 32 of 32 [%100.00] (wr 0.045994 sec, rd 0.039005 sec)
thread averages:
  1 (wr 0.212761 sec, rd 0.190548 sec)
  2 (wr 0.153349 sec, rd 0.124817 sec)
  3 (wr 0.104419 sec, rd 0.077651 sec)
  4 (wr 0.046590 sec, rd 0.019056 sec)
total average: (wr 2.068476 sec, rd 1.648289 sec)
benched 16.00 GiB worth of memory (585.14 MiB/s) in 27.234011 secs total
```

#### i7-4790K
```
discovered Intel(R) Core(TM) i7-4790K CPU @ 4.00GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 4 thread(s) per trial run
trial 32 of 32 [%100.00] (wr 1.698562 sec, rd 1.356559 sec)
thread averages:
  1 (wr 0.282012 sec, rd 0.255565 sec)
  2 (wr 0.228344 sec, rd 0.197634 sec)
  3 (wr 0.158749 sec, rd 0.118875 sec)
  4 (wr 0.082220 sec, rd 0.039247 sec)
total average: (wr 3.005302 sec, rd 2.445285 sec)
benched 16.00 GiB worth of memory (442.81 MiB/s) in 36.098130 secs total
```

#### i7-6700k
```
discovered Intel(R) Core(TM) i7-6700K CPU @ 4.00GHz: 8 logical CPU(s), 4 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 4 thread(s) per trial run
trial 32 of 32 [%100.00] (wr 1.415474 sec, rd 1.096290 sec)
thread averages:
  1 (wr 0.211443 sec, rd 0.188549 sec)
  2 (wr 0.163605 sec, rd 0.136219 sec)
  3 (wr 0.109080 sec, rd 0.078811 sec)
  4 (wr 0.051359 sec, rd 0.023321 sec)
total average: (wr 2.141946 sec, rd 1.707605 sec)
benched 16.00 GiB worth of memory (585.14 MiB/s) in 27.065301 secs total
```

### 8 Thread Configurations

#### Ryzen 1700
```
discovered AMD Ryzen 7 1700 Eight-Core Processor: 16 logical CPU(s), 8 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread(s) per trial run
trial 32 of 32 [%100.00] (wr 3.409272 sec, rd 3.044805 sec)
thread averages:
  1 (wr 0.376523 sec, rd 0.358443 sec)
  2 (wr 0.329873 sec, rd 0.306196 sec)
  3 (wr 0.283812 sec, rd 0.261372 sec)
  4 (wr 0.235190 sec, rd 0.210351 sec)
  5 (wr 0.183385 sec, rd 0.159213 sec)
  6 (wr 0.132993 sec, rd 0.112440 sec)
  7 (wr 0.084588 sec, rd 0.063350 sec)
  8 (wr 0.038941 sec, rd 0.016044 sec)
total average: (wr 13.322433 sec, rd 11.899265 sec)
benched 32.00 GiB worth of memory (337.81 MiB/s) in 96.391158 secs total
```

#### Xeon E5-2650 v1 (2x4GiB DDR3 1333 MhZ)
```
discovered Intel(R) Xeon(R) CPU E5-2650 0 @ 2.00GHz: 16 logical CPU(s), 8 physical, 2 thread(s) per core
measuring memory perf across CPU(s) with explicit memory mappings
running 32 trial(s) on a space of 128.00 MiB with 8 thread(s) per trial run
trial 32 of 32 [%100.00] (wr 3.420858 sec, rd 3.014795 sec)
thread averages:
  1 (wr 0.355105 sec, rd 0.334069 sec)
  2 (wr 0.315440 sec, rd 0.296332 sec)
  3 (wr 0.275131 sec, rd 0.256213 sec)
  4 (wr 0.234123 sec, rd 0.208792 sec)
  5 (wr 0.191269 sec, rd 0.168584 sec)
  6 (wr 0.145721 sec, rd 0.123997 sec)
  7 (wr 0.099425 sec, rd 0.071881 sec)
  8 (wr 0.047795 sec, rd 0.022334 sec)
total average: (wr 13.312079 sec, rd 11.857610 sec)
benched 32.00 GiB worth of memory (360.08 MiB/s) in 90.909019 secs total
```
