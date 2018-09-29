#include "../include/funciones.h"


double **create_array( int Row, int Col )
{
    double **array = new double*[Row];
    for ( int i = 0; i < Row; i++ )
        array[i] = new double[Col];
    return array;
}


double Random( int High, int Low )
{
    time_t segs;
   
    srand( ( unsigned int )(segs % 65536) );
   
    return (((double)rand()/RAND_MAX) * (High - Low) + Low);
}

void mostrar_matriz(double **matriz, int filas, int cols)
{
    for(int i = 0; i <filas; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            cout<<"Value ["<<i<<"] ["<<j<<"]: "<<matriz[i][j]<<endl;
        }

    }
}


double sumatoria_de_pesos(int cant, double *peso, double *ent)
{
    int i;
    double sum = 0.0;
    for(i = 0; i<cant; i++)
    {
        sum += peso[i]*ent[i];
    }

    return sum;
}

double funcion_activacion(double potencial_sinaptico,int tipo)
{
    double salida = 0.0;
    switch(tipo)
    {
        case 0:
            
            salida = potencial_sinaptico;
            break;
        case 1:
           
            salida = 1/(1+ (exp(-1*potencial_sinaptico)));
            break;
        case 2:
            
            if(potencial_sinaptico>0.0)
                salida = 1;
            else
                salida = 0;
            break;
        case 3:
            
            salida = exp(potencial_sinaptico*potencial_sinaptico);
            break;
        default:
            
            cout<<"The function is not defined.It will take the default linear function";
            salida = potencial_sinaptico;
    }
    return salida;

}
