#!/usr/bin/env gnuplot
fn="Anass_metrics_data_1.csv"

#set terminal pngcairo size 800,500
set terminal svg size 800,600
ext="svg"
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
set title "Pixel Density (|px|/|px^2|)"
unset ylabel 
unset y2label 
set grid
set logscale y

set yrange [0.05:*]
#set y2range [0:*]
set ytics nomirror
unset y2tics

#set ylabel "with DDM++"
#set y2label "without DDM++"

unset ylabel
unset y2label
set xlabel "# Frame"

set output "density.".ext
plot \
	fn using 1:6 ti "with DDM++" lw 2,\
	fn using 1:7 ti "without DDM++" lw 2

unset logscale y
	
#-------------------------------------------------
set title "Ratio MAE" font "Helvetica,20"
set output "ratio_lin_mae.".ext
plot \
	fn using 1:(column(2)/column(7)) ti "MAE/density without DDM++" lw 2, \
	fn using 1:(column(4)/column(6)) ti "MAE/density with DDM++" lw 2


set title "Ratio MED"
set output "ratio_lin_med.".ext
plot \
	fn using 1:(column(3)/column(7)) ti "MED/density without DDM++" lw 2, \
	fn using 1:(column(5)/column(6)) ti "MED/density with DDM++" lw 2


set logscale y

set title "Ratio MAE (log)"
set output "ratio_log_mae.".ext
plot \
	fn using 1:(column(2)/column(7)) ti "MAE/density without DDM++" lw 2, \
	fn using 1:(column(4)/column(6)) ti "MAE/density with DDM++" lw 2


set title "Ratio MED (log)"
set output "ratio_log_med.".ext
plot \
	fn using 1:(column(3)/column(7)) ti "MED/density without DDM++" lw 2, \
	fn using 1:(column(5)/column(6)) ti "MED/density with DDM++" lw 2



