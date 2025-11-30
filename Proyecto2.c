#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535897932384f
#define M 2000
#define CanSen 6

void arreglos(double xn[], double A, int NuCiclos){
    // Se define el angulo y la amplitud
    double Wo = (2 * PI * NuCiclos) / M;
    // Se levantan los arreglos que va a tener nuestro oscilador
    double valorCos;
    valorCos = cos(Wo);
    xn[0] = A;
    xn[1] = A * valorCos;
    for(int i=2; i<M;i++)
        xn[i] = (2.0 * valorCos * xn[i-1]) - xn[i-2];
}

void filtros(double SumS[], double filt1[], double a[], double b[]){
    // Se contemplan los primeros cuatro factores
    for(int n =0; n<4;n++){
        if (!n)
            filt1[0] = (b[0]* SumS[n]);
        else{ 
            filt1[n] = (b[0]* SumS[n]);
            for(int k = 1; k<(n+1) ; k++){     
                filt1[n] += ((b[k]* SumS[n-k]) - (a[k]*filt1[n-k]));
            }
        }
    }
    // Se realiza la ecuacion de diferencias del filtro
    for(int n=4; n<M; n++)
        filt1[n] = ((b[0] * SumS[n]) + (b[1] * SumS[n-1]) + (b[2] * SumS[n-2]) + (b[3] * SumS[n-3]) + (b[4] * SumS[n-4])) - ((a[1] * filt1[n-1]) + (a[2] * filt1[n-2]) + (a[3] * filt1[n-3])+ (a[4] * filt1[n-4]));  
}

void Groetzel(double xn[], double Groetzel1[]){
    double sumarR;
    double sumarI;
    double V1, V2;
    double vk;
    double coseno;

    for(int k=0; k<M;k++){
        V1 = 0.0;
        V2 = 0.0;
        coseno= 2.0 * cos((2.0 * PI * k)/M);
        for(int n = 0; n < M; n++){
            vk = xn[n] + ( coseno * V1) - V2;
            V2 = V1;
            V1 = vk;  
        }
        sumarR =  (cos((2.0 * PI * k)/M) * V1) - V2 ;
        sumarI = (sin((2.0 * PI * k)/M)) * V1;
        Groetzel1[k] = sqrt((sumarR * sumarR) + (sumarI * sumarI));
    }
}

int main()
{
    double xn[M], xn1[M],xn2[M],xn3[M],xn4[M],xn5[M];
    double filt1[M];
    double dft1[M];
    double Groet1[M];
    double f0 = 15.0; 
    // Para sumar varias señales
    double* senales[CanSen] = {xn,xn1,xn2,xn3,xn4,xn5};
    //double Amplitudes[CanSen] = {1.0f, 1.0f};
    double frecuencia[CanSen] = {f0,2.0*f0,3.0*f0,4.0*f0,5.0*f0,6.0*f0};
    for(int i =0; i<CanSen; i++)
        arreglos(senales[i], 1.0, frecuencia[i]);
    double SumS[M];
    for(int n =0;n<M; n++)
        SumS[n] = xn[n] + xn1[n] + xn2[n]+ xn3[n] + xn4[n]+ xn5[n];     

    // Se obtiene su transformada antes de los filtros
    Groetzel(SumS,Groet1);

    // Se imprimen las señales 
    FILE *sena1234 = fopen("SumS.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(sena1234,"%f\n" ,SumS[i]);
    fclose(sena1234);  // cerrar el archivo

    // Se imprime el primer espectro antes de los filtros 
    FILE *GroSum = fopen("espec.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(GroSum,"%f\n" ,Groet1[i]);
    fclose(GroSum);  // cerrar el archivo
    
    
    // Las constantes que hacen referencia al primer filtro
    double b[] = {0.978030479206564,  -3.89524032741485 ,   5.83449254385514 ,  -3.89524032741485 ,   0.978030479206564};
    double a[] = {1.0000 ,  -3.93850912816633    ,5.83400982575703,   -3.85197152666332,    0.956543676511202};
    
    // Se ejecuta el primer filtro
    filtros(SumS, filt1, a,b);

    // Se ejecuta el segundo filtro
    double filtr2[M];
    // Las constantes del segundo filtro
    double b3[] = { 0.978030479206561 , -3.84330162077788  ,5.7317529389578   ,-3.84330162077788   , 0.978030479206561 };
    double a3[] = {1.0000 ,  -3.88599348009321, 5.73127022085972  , -3.80060976146257   ,  0.956543676511209 };
    // Se ejecuta el segundo filtro
    filtros(filt1, filtr2, a3,b3);
/*
    double Filtro1[M];
    Filtro1[0] = b[0];
    Filtro1[1] = -(a[1] *Filtro1[0])  + b[1];
    Filtro1[2] = -(a[1] * Filtro1[1]) - (a[2]* Filtro1[0])  + b[2];
    Filtro1[3] = -(a[1] * Filtro1[2]) - (a[2]* Filtro1[1]) - (a[3]* Filtro1[0])  + b[3];
    Filtro1[4] = -(a[1] * Filtro1[3]) - (a[2]* Filtro1[2]) - (a[3]* Filtro1[1]) - (a[4]* Filtro1[0])  + b[4];
    for(int i= 4; i<M; i++)
        Filtro1[i] = -(a[1] * Filtro1[i-1]) - (a[2]* Filtro1[i-2]) - (a[3]* Filtro1[i-3]) - (a[4]* Filtro1[i-4]) ; 
*/


    Groetzel(filtr2,Groet1);
    // Se imprime las señales filtradas

    FILE *filSum = fopen("filsum.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(filSum,"%f\n" ,filtr2[i]);
    fclose(filSum);  // cerrar el archivo

    // Se imprime despues del filtro 
    FILE *archivoCK = fopen("xn2.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(archivoCK,"%f\n" ,Groet1[i]);
    fclose(archivoCK);  // cerrar el archivo
    
/*    
    FILE *arhi2 = fopen("xn1.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(arhi2,"%f\n" ,dft1[i]);
    fclose(arhi2);  // cerrar el archivo
*/

    system("gnuplot -p grafi.gp");

    return 0;
}
