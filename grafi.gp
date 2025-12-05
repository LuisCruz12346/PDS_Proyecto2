reset
set grid
set autoscale
#------------- Graf 1 ----------------
set style data lines
set title "Señal de entrada x[n]"
#unset label
set xlabel   "N"
set ylabel   "x[n]"
plot  "senalEntrada.dat"
pause -1 "Grafica 1: Cerrar para ver siguiente\n"

#------------- Graf 2 ----------------
set style data lines
set title "Espectro de amplitud de señal de entrada X[k]"
#unset label
set xlabel   "N"
set ylabel   "X[K]"
plot  "espectroSenalEntrada.dat"
pause -1 "Grafica 2: Cerrar para ver siguiente\n"

#------------- Graf 3 ----------------
set title "Señal con primer filtro y[n]"
#unset label
set xlabel   "N"
set ylabel   "y[n]"
plot  "senalFiltrada_1.dat"    
pause -1 "Grafica 3: Cerrar para ver siguiente\n"

#------------- Graf 4 ----------------
set style data lines
set title "Espectro de amplitud de señal con primer filtro Y[K]"
#unset label
set xlabel   "N"
set ylabel   "Y[k]"
plot  "espectroSenalFiltrada_1.dat" with impulses
pause -1 "Grafica 4: Cerrar para ver siguiente\n"

#------------- Graf 5 ----------------
set title "Señal con segundo filtro y[n]"
#unset label
set xlabel   "N"
set ylabel   "y[n]"
plot  "senalFiltrada_2.dat"    
pause -1 "Grafica 5: Cerrar para ver siguiente\n"


#------------- Graf 6 ----------------
set style data lines
set title "Espectro de amplitud de señal totalmente filtrada Y[K]"
#unset label
set xlabel   "N"
set ylabel   "Y[k]"
plot  "espectroSenalFiltrada_2.dat" with impulses
pause -1 "Grafica 6: Cerrar para ver siguiente\n"

#------------- Graf 7 ----------------
set title "Respuesta al impulso del primer Filtro "
#unset label
set xlabel   "N"
set ylabel   "H[K]"
plot  "filtro1.dat"    
pause -1 "Grafica 7: Cerrar para ver siguiente\n"

#------------- Graf 8 ----------------
set title "Respuesta al impulso del Segundo Filtro "
#unset label
set xlabel   "N"
set ylabel   "H[K]"
plot  "filtro2.dat"    
pause -1 "Grafica 8: Cerrar para terminar el programa\n"
