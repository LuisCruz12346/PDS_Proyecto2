reset
set grid
set autoscale
#------------- Graf 1 ----------------
set style data lines
set title "Suma de senales"
#unset label
set xlabel   "N"
set ylabel   "x[n]"
plot  "SumS.dat"
pause -1 "Grafica 1: Cerrar para ver siguiente\n"

#------------- Graf 2 ----------------
set style data lines
set title "Groetzel"
#unset label
set xlabel   "N"
set ylabel   "X[K]"
plot  "espec.dat"
pause -1 "Grafica 2: Cerrar para ver siguiente\n"

#------------- Graf 3 ----------------
set title "Senales despues del sistema"
#unset label
set xlabel   "N"
set ylabel   "y[n]"
plot  "filsum.dat"    
pause -1 "Grafica 3: Cerrar para ver siguiente\n"

#------------- Graf 4 ----------------
set style data lines
set title "Groetzel"
#unset label
set xlabel   "N"
set ylabel   "Y[k]"
plot  "xn2.dat" with impulses
pause -1 "Grafica 4: Cerrar para terminar el programa\n"

