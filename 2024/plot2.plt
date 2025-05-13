#!/usr/bin/env gnuplot
fn="Anass_metrics_data_1.csv"

# ----------------------------------
# au choix, svg ou png
set terminal svg size 1000,700
ext="svg"

#set terminal pngcairo size 800,1000
#ext="png"
# ----------------------------------

set xrange [0:130]
set key left top

set datafile separator ","
set output "test1.".ext
set grid

set style data linespoints
set ylabel "Error value (m.)"
set y2tics
set y2label "Pixel Density (px/px^2)"

plot \
	fn using 1:2 ti "MAE without DDM++" axis x1y1 \
	, fn using 1:3 ti "MED without DDM++" \
	, fn using 1:4 ti "MAE with DDM++" \
	, fn using 1:5 ti "MED with DDM++" \
	, fn using 1:6 axes x1y2 ti "Density with DDM++"  \
	, fn using 1:7 axes x1y2  ti "Density before DDM++" 


#-------------------------------------------------
set title "Depth Map Density" font "Helvetica,24"

unset ylabel 

set grid
set logscale y

set yrange [0.05:*]
set xrange [0:13]
set ytics nomirror
unset y2tics
unset y2label


set xlabel "Time (s.)" font "Helvetica,18"
set ylabel "Density (%)" font "Helvetica,18"

set ytics autofreq font "Helvetica,16"
set xtics autofreq font "Helvetica,16"

set key font "Helvetica,20"

set output "density.".ext
plot \
	fn using (column(1)/10.):6 ti "with DDM++" lw 3,\
	fn using (column(1)/10.):7 ti "without DDM++" lw 3

unset logscale y
	
#-------------------------------------------------
set title "Ratio MAE (lin)" font "Helvetica,20"
set output "ratio_lin_mae.".ext
plot \
	fn using 1:(column(2)/column(7)) ti "MAE/density without DDM++" lw 2, \
	fn using 1:(column(4)/column(6)) ti "MAE/density with DDM++" lw 2


set title "Ratio MED"
set output "ratio_lin_med.".ext
plot \
	fn using 1:(column(3)/column(7)) ti "MED/density without DDM++" lw 2, \
	fn using 1:(column(5)/column(6)) ti "MED/density with DDM++" lw 2

#===============================================================

set key left top
set output "ratio_log_MAE.".ext
set xrange [0:13]
#set multiplot layout 2,1

set ylabel "MAE (m.)" font "Helvetica,18"

set logscale y

set title "MAE/Density ratio"
plot \
	fn using (column(1)/10.):(column(2)/column(7)) ti "MAE/density without DDM++" lw 3, \
	fn using (column(1)/10.):(column(4)/column(6)) ti "MAE/density with DDM++" lw 3


set output "ratio_log_MED.".ext

set title "MED/Density ratio"
set yrange [0.02:*]
#set size 0.4,1
#set origin 0.5,0
#set output "ratio_log_med.".ext
set ylabel "MED (m.)" font "Helvetica,18"

plot \
	fn using (column(1)/10.):(column(3)/column(7)) ti "MED/density without DDM++" lw 3, \
	fn using (column(1)/10.):(column(5)/column(6)) ti "MED/density with DDM++" lw 3


#unset multiplot



