#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535897932384f
#define M 500

void arreglos(float xn[], float A, int NuCiclos){
    // Se define el angulo y la amplitud
    float Wo = (2 * PI * NuCiclos) / M;
    // Se levantan los arreglos que va a tener nuestro oscilador
    float valorCos;
    valorCos = cosf(Wo);
    xn[0] = A;
    xn[1] = A * valorCos;
    for(int i=2; i<M;i++)
        xn[i] = (2.0f * valorCos * xn[i-1]) - xn[i-2];
}

void DFT(float xn[], float Dft1[]){
    float sumatoriaR;
    float sumatoriaIm;
    for(int i=0; i<M;i++){ // Obtener X[K]
        sumatoriaR=0.0f;
        sumatoriaIm=0.0f;
        for(int n=0;n<M;n++){ // Obtener Sumatoria
            sumatoriaR += (xn[n]* cosf(2.0f * PI * (float)i * (float)n / (float)M));
            sumatoriaIm += (xn[n]* sinf(2.0f * PI * (float)i * (float)n / (float)M));  
        }
        Dft1[i]=sqrtf( (sumatoriaR * sumatoriaR) + (sumatoriaIm * sumatoriaIm));
    }
}

void Groetzel(float xn[], float Groetzel1[]){
    float sumarR;
    float sumarI;
    float V1, V2;
    float vk;

    for(int k=0; k<M;k++){
        V1 = 0.0f;
        V2 = 0.0f;
        for(int n = 0; n < M; n++){
            vk = xn[n] + (2.0f * cosf((2 * PI * k)/M) * V1) - V2;
            V2 = V1;
            V1 = vk;  
        }
        sumarR = V1 - ((cosf((2.0f * PI * k)/M)) * V2);
        sumarI = (sinf((2.0f * PI * k)/M)) * V2;
        Groetzel1[k] = sqrtf((sumarR * sumarR) + (sumarI * sumarI));
    }
}

int main()
{
    float xn1[M], xn2[M];
    float dft1[M];
    float Groet1[M];
    // Para sumar varias señales
    float* senales[2] = {xn1,xn2};
    float Amplitudes[2] = {1.0f, 1.0f};
    float frecuencia[2] = {100.0f,50.0f};
    for(int i =0; i<2; i++)
        arreglos(senales[i], Amplitudes[i], frecuencia[i]);
    float SumS[M];
    for(int n =0;n<M; n++)
        SumS[n] = xn1[n] + xn2[n];     

    //arreglos(xn1, 1.0f, 50);

    //DFT(SumS, dft1);

    Groetzel(SumS,Groet1);

    // Se imprimen las señales 
    FILE *archivoCK = fopen("xn1.dat", "w");;  // puntero al archivo
    for(int i =0; i< M; i++)
        fprintf(archivoCK,"%f\n" ,Groet1[i]);
    fclose(archivoCK);  // cerrar el archivo

    system("gnuplot -p grafi.gp");

    return 0;
}
