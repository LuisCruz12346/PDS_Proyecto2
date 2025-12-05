#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 2000

/**
 * @brief      Genera una señal sinusoidal mediante un oscilador digital
 *
 * @param[out] xn        Arreglo que contendra la señal
 * @param[in]  A         Amplitud de la señal
 * @param[in]  numCiclos Numero de ciclos de la señal en los M puntos
 */
void generarOscilador(double xn[], double A, int numCiclos){
    // Se define el angulo y la amplitud
    double Wo = (2 * M_PI * numCiclos) / M;
    // Se levantan los arreglos que va a tener nuestro oscilador
    double valorCos;
    valorCos = cos(Wo);
    xn[0] = A;
    xn[1] = A * valorCos;
    for(int i=2; i<M;i++)
        xn[i] = (2.0 * valorCos * xn[i-1]) - xn[i-2];
}

/**
 * @brief      Aplica un filtro paso banda IIR con los coeficientes a's y b's proporcionados
 *
 * @param      senalEntrada[in]     Señal a la que se aplicará el filtro
 * @param      senalFiltrada[out]   Señal filtrada
 * @param      a                    Coeficientes a's del filtro IIR
 * @param      b                    Coeficientes b's del filtro IIR
 */
void aplicarFiltroIIR(double senalEntrada[], double senalFiltrada[], double a[], double b[]){
    // Se contemplan los primeros cuatro factores
    for(int n = 0; n < 4;n++){
        if (!n)
            senalFiltrada[0] = (b[0]* senalEntrada[n]);
        else{ 
            senalFiltrada[n] = (b[0]* senalEntrada[n]);
            for(int k = 1; k<(n+1) ; k++){     
                senalFiltrada[n] += ((b[k]* senalEntrada[n-k]) - (a[k]*senalFiltrada[n-k]));
            }
        }
    }
    // Se aplica la ecuacion de diferencias del filtro
    for(int n=4; n<M; n++)
        senalFiltrada[n] = ((b[0] * senalEntrada[n]) + (b[1] * senalEntrada[n-1]) + (b[2] * senalEntrada[n-2]) + (b[3] * senalEntrada[n-3]) + (b[4] * senalEntrada[n-4])) - ((a[1] * senalFiltrada[n-1]) + (a[2] * senalFiltrada[n-2]) + (a[3] * senalFiltrada[n-3])+ (a[4] * senalFiltrada[n-4]));  
}

/**
 * @brief      Se modela la respuesta al impulso de cada filtro
 *
 * @param      Filtro1[out]         Senal que representa el H del filtro
 * @param      a                    Coeficientes a's del filtro IIR
 * @param      b                    Coeficientes b's del filtro IIR
 */

void RespuestaImpulsoFiltro(double Filtro1[], double a[], double b[]){
    Filtro1[0] = b[0];
    Filtro1[1] = -(a[1] *Filtro1[0])  + b[1];
    Filtro1[2] = -(a[1] * Filtro1[1]) - (a[2]* Filtro1[0])  + b[2];
    Filtro1[3] = -(a[1] * Filtro1[2]) - (a[2]* Filtro1[1]) - (a[3]* Filtro1[0])  + b[3];
    Filtro1[4] = -(a[1] * Filtro1[3]) - (a[2]* Filtro1[2]) - (a[3]* Filtro1[1]) - (a[4]* Filtro1[0])  + b[4];
    for(int i= 5; i<M; i++)
        Filtro1[i] = -(a[1] * Filtro1[i-1]) - (a[2]* Filtro1[i-2]) - (a[3]* Filtro1[i-3]) - (a[4]* Filtro1[i-4]) ;
}

/**
 * @brief      Obtiene el espectro de magnitud de x[n]
 *
 * @param[in]       xn                  Señal x[n] a la que se le quiere sacar el espectro de magnitud
 * @param[out]      espectroMagnitud    Espectro de magnitud se x[n]
 */
void Goertzel(double xn[], double espectroMagnitud[]){
    double sumarR;
    double sumarI;
    double V1, V2;
    double vk;
    double coseno;

    for(int k=0; k<M;k++){
        V1 = 0.0;
        V2 = 0.0;
        coseno= 2.0 * cos((2.0 * M_PI * k)/M);
        for(int n = 0; n < M; n++){
            vk = xn[n] + ( coseno * V1) - V2;
            V2 = V1;
            V1 = vk;  
        }
        sumarR =  (cos((2.0 * M_PI * k)/M) * V1) - V2 ;
        sumarI = (sin((2.0 * M_PI * k)/M)) * V1;
        espectroMagnitud[k] = sqrt((sumarR * sumarR) + (sumarI * sumarI));
    }
}

int main()
{
    // Arreglos para señal a frecuencia fundamental y sus armónicos
    double xn0[M], xn1[M],xn2[M],xn3[M],xn4[M],xn5[M];
    // Suma de señales (señal que se filtrará)
    double sum_xn[M];
    double senalFiltrada_1[M];
    double senalFiltrada_2[M];
    double espectroMagnitud[M];
    double f0 = 15.0; 

    generarOscilador(xn0, 1.0, f0);
    generarOscilador(xn1, 1.0, 2.0*f0);
    generarOscilador(xn2, 1.0, 3.0*f0);
    generarOscilador(xn3, 1.0, 4.0*f0);
    generarOscilador(xn4, 1.0, 5.0*f0);
    generarOscilador(xn5, 1.0, 6.0*f0);

    // Sumar señales para generar la entrada
    for(int n = 0;n < M; n++)
        sum_xn[n] = xn0[n] + xn1[n] + xn2[n]+ xn3[n] + xn4[n]+ xn5[n];     

    FILE *archivoSenalEntrada = fopen("senalEntrada.dat", "w");
    for(int i =0; i< M; i++)
        fprintf(archivoSenalEntrada,"%f\n" ,sum_xn[i]);
    fclose(archivoSenalEntrada);

    // Generar espectro de magnitud de señal de entrada
    Goertzel(sum_xn, espectroMagnitud);

    // Guardar espectro de magnitud de señal de entrada
    FILE *archivoEspectroSenalEntrada = fopen("espectroSenalEntrada.dat", "w");
    for(int i =0; i< M; i++)
        fprintf(archivoEspectroSenalEntrada,"%f\n" ,espectroMagnitud[i]);
    fclose(archivoEspectroSenalEntrada);
    
    
    // Constantes del primer filtro
    double b_1[] = {0.978030479206564,  -3.89524032741485 ,   5.83449254385514 ,  -3.89524032741485 ,   0.978030479206564};
    double a_1[] = {1.0000 ,  -3.93850912816633    ,5.83400982575703,   -3.85197152666332,    0.956543676511202};
    aplicarFiltroIIR(sum_xn, senalFiltrada_1, a_1, b_1);
    
    FILE *archivoSenalFiltrada_1 = fopen("senalFiltrada_1.dat", "w");
    for(int i =0; i< M; i++)
        fprintf(archivoSenalFiltrada_1,"%f\n" ,senalFiltrada_1[i]);
    fclose(archivoSenalFiltrada_1);

    Goertzel(senalFiltrada_1, espectroMagnitud);

    // Guardar espectro de señal filtrada
    FILE *archivoEspectroSenalFiltrada_1 = fopen("espectroSenalFiltrada_1.dat", "w");
    for(int i =0; i< M; i++)
        fprintf(archivoEspectroSenalFiltrada_1,"%f\n" ,espectroMagnitud[i]);
    fclose(archivoEspectroSenalFiltrada_1);

    // Constantes del segundo filtro
    double b_2[] = { 0.978030479206561 , -3.84330162077788  ,5.7317529389578   ,-3.84330162077788   , 0.978030479206561 };
    double a_2[] = {1.0000 ,  -3.88599348009321, 5.73127022085972  , -3.80060976146257   ,  0.956543676511209 };
    aplicarFiltroIIR(senalFiltrada_1, senalFiltrada_2, a_2,b_2);
    // Generar espectro de magnitud de señal filtrada
    Goertzel(senalFiltrada_2, espectroMagnitud);

    // Guardar señal filtrada
    FILE *archivoSenalFiltrada_2 = fopen("senalFiltrada_2.dat", "w");
    for(int i =0; i< M; i++)
        fprintf(archivoSenalFiltrada_2,"%f\n" ,senalFiltrada_2[i]);
    fclose(archivoSenalFiltrada_2);

    // Guardar espectro de señal filtrada
    FILE *archivoEspectroSenalFiltrada_2 = fopen("espectroSenalFiltrada_2.dat", "w");
    for(int i =0; i< M; i++)
        fprintf(archivoEspectroSenalFiltrada_2,"%f\n" ,espectroMagnitud[i]);
    fclose(archivoEspectroSenalFiltrada_2);

    // Se grafican la respuesta al impulso de cada filtro    
    // Filtro para suprimir Fo1
    double Filtro1[M];
    RespuestaImpulsoFiltro(Filtro1,a_1,b_1);
    Goertzel(Filtro1,espectroMagnitud);
    // Se guarda el espectro del filtro
    FILE *fil1 = fopen("filtro1.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(fil1,"%f\n" ,espectroMagnitud[i]);
    fclose(fil1);  // cerrar el archivo
    
    // Filtro para suprimir Fo3
    double Filtro2[M];
    RespuestaImpulsoFiltro(Filtro2,a_2,b_2);
    Goertzel(Filtro2,espectroMagnitud);
    // Se guarda el espectro del filtro
    FILE *fil2 = fopen("filtro2.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(fil2,"%f\n" ,espectroMagnitud[i]);
    fclose(fil2);  // cerrar el archivo
    
    system("gnuplot -p grafi.gp");
    return 0;
}
