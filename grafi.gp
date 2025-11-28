#
#  Configura graficaciÃ³n en GNUplot
#  
# Ejecutar desde un Shell de linux $ gnuplot -p 'graf_senal.gp
# Ejecutar desde GNUplot; $load 'graf_senal.gp'
# Ejecutar desde una lÃ­nea en cÃ³digo  c:
# system("gnuplot -p 'graf_senal.gp' ");
###
set autoscale
set output  "seno.eps"
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Senal de entrada"
#unset label
set xlabel   "nw"
set ylabel   "phi(nw)"
plot  "xn2.dat" 
unset xlabel
unset ylabel
unset title