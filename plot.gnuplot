#!/usr/bin/gnuplot

set term postscript eps enhanced "Times-Roman"
set key left
set border 3 lt 0
set size 0.8,0.8
set style data linespoints

set ylabel "Delay"
set xlabel "Load %"
set output "Load-Delay.eps"
plot [0:100] "data.txt" using 1:2 w lp smooth csplines title "with reroute", \
     "" using 1:3 w lp smooth csplines title "no reroute"

set ylabel "Loss %"
set xlabel "Load %"
set output "Load-Loss.eps"
plot [0:100] [0:15] "data.txt" using 1:4 w lp title "with reroute", \
     "" using 1:5 w lp title "no reroute"
