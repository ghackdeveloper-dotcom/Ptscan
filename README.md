# Ptscan

ptscan is a port scanner built in C and can be compiled on Linux, not Windows. I'd add that this project is still under development, 
so there will be both efficiency and timing updates, plus the Windows version. Thank you

To compile it: gcc ptscan -o ptscan

Useage:

./ptscan <ip> -s <initial_port> <final_port>

./ptscan <ip> -p <port>

./ptscan <ip> -a (all scan)
