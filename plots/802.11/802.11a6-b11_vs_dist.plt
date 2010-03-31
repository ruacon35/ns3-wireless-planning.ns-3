set terminal png
set output 'WiFi-a6-b11_vs_Dist_Throughput.png'
set title 'WiFi for Long Distances'
set xlabel 'Time [sec]'
set ylabel 'Throughput [Mbps]'
plot '-'  title '802.11b 11Mbps' with lines, '-'  title '802.11a 6Mbps' with lines
0		9
1		8.85
3		8.50
5		8.10
10		7.25
15		6.75
20		6.15
30		5.40
40		4.75
50		4.25
70		3.5		
90		3		
110		2.6	
150		2		
e
0		5.5
1			5.4
3			5.25
5			5.1
10		4.8
15	4.75
20		4.35
30		3.75
40			3.5
50			3.3
70				2.75
90			2.4
110			2.2
150		1.75
e
