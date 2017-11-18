set term png size 1920,1080
set output "result.png"

set format x "%.2f"
set format y "%.2f"

set ylabel "Bandwidth (MB/s)"
set xlabel "Time (secs)"

set xrange [0:600]
set yrange [0:2100]

set ytics 0,100

set key left top
set title "Results"

plot "results/data.8"   using 1:2   title "8 Threads"   with linespoints pt 5 lc rgb 'red', \
     ""                 using 1:2:3 title ""            with labels right, \
     "results/data.12"  using 1:2   title "12 Threads"  with linespoints pt 5 lc rgb 'green', \
     ""                 using 1:2:3 title ""            with labels right, \
     "results/data.16"  using 1:2   title "16 Threads"  with linespoints pt 5 lc rgb 'blue', \
     ""                 using 1:2:3 title ""            with labels right, \
     "results/data.56"  using 1:2   title "56 Threads"  with linespoints pt 5 lc rgb 'magenta', \
     ""                 using 1:2:3 title ""            with labels right, \
     "results/data.64"  using 1:2   title "64 Threads"  with linespoints pt 5 lc rgb 'yellow', \
     ""                 using 1:2:3 title ""            with labels right, \
     "results/data.256" using 1:2   title "256 Threads" with linespoints pt 5 lc rgb 'cyan', \
     ""                 using 1:2:3 title ""            with labels right
