#include "../include/CapaNeuronal.h"
#include <stdio.h>

CapaNeuronal::CapaNeuronal()
{
}

void CapaNeuronal::New(int cant_deneuronas, CapaNeuronal *ant, FILE *arch)
{
    this->cant_neuronas = cant_deneuronas;
    this->anterior = ant;

    
    this->CreateLayer();
    if (arch!=NULL&&anterior!=NULL)
    {
        this->LoadLayer(arch);
    }
}

void CapaNeuronal::KnowingWithBeforeLayer()
{
    this->anterior->SetNextLayer(this);
}

void CapaNeuronal::CreateLayer()
{
    if(anterior==NULL)
    {
        this->cant_entradas = cant_neuronas;
        this->entrada = new double[cant_entradas];
        this->salida = this->entrada;
    }
    else if(anterior!=NULL)
    {
      
        this->KnowingWithBeforeLayer();
        this->cant_entradas = this->anterior->GetCantNeuronas();
        this->salida = new double[cant_neuronas];
        this->Umbral = new double[cant_neuronas];
        this->DeltaH = new double[cant_neuronas];
        this->SetRandomWeight();
        this->entrada = new double[cant_entradas];
    }
    this->Momento = 0.5;
    this->siguiente = NULL;
    this->Epsilon = -0.05;


}

CapaNeuronal::~CapaNeuronal()
{
    
    delete [] entrada;
    if(anterior!=NULL)
    {
        delete [] DeltaH;
        delete [] *weights;
        delete [] salida;
        delete [] this->Umbral;
    }

}

double CapaNeuronal::GetCantNeuronas(void)
{
	return(this->cant_neuronas);
}

double CapaNeuronal::GetSalida(int neurona)
{
	if(neurona > this->cant_neuronas)
	{
		return 0;
	}
	else
	{
	return this->salida[neurona];
	}
}

void CapaNeuronal::SetNextLayer(CapaNeuronal *Sig)
{
    this->siguiente = Sig;
}

void CapaNeuronal::SetRandomWeight()
{
    int i,j;
    this->weights = new double*[cant_neuronas];
    this->dweights = new double*[cant_neuronas];
    for(i = 0; i<cant_neuronas; i++)
    {
        this->weights[i] = new double[cant_entradas];
        this->dweights[i] = new double[cant_entradas];
        for(j = 0; j<cant_entradas; j++)
        {
            cout<< "Neuron weights ("<<i;
            cout<<", "<<j<<") :";
            this->weights[i][j] = Random(2, -2);
            cout<<this->weights[i][j]<<endl;
        }
    }

}

void CapaNeuronal::ProcessOutput()
{
    if(anterior!=NULL)
    {
        this->GetInputFromBackLayer();
        int i ,j;
        for(i=0; i<cant_neuronas;i++)
        {
            this->salida[i] = sumatoria_de_pesos(this->cant_entradas, this->weights[i], this->entrada);
            this->salida[i] += this->Umbral[i];
            this->salida[i] = funcion_activacion(this->salida[i], 1);
        }

    }
}

void CapaNeuronal::GetInputFromBackLayer()
{

    int i;
    for(i = 0; i<this->cant_entradas; i++)
    {
        this->entrada[i] = this->anterior->GetSalida(i);

    }
}


void CapaNeuronal::ComputeError()
{
    this->Error_capa = 0.0f;
    int i,j, neuronas_capa_siguiente = this->siguiente->GetCantNeuronas();

    for(i=0; i<this->cant_neuronas; i++)
    {
        
        double Error_tmp = 0.0f;
        for(j=0; j<neuronas_capa_siguiente; j++)
        {
            Error_tmp += this->siguiente->GetDeltaH(j)*this->siguiente->GetPeso(j, i);
            

            this->DeltaH[i] = (1 - this->salida[i]) * this->salida[i] * Error_tmp;
            
        }

    }

}

double CapaNeuronal::ComputeError(double *Target)
{
    this->Error_capa = 0.0;
    int i,j;
    double Error_tmp = 0.0;
    for(j=0; j<this->cant_neuronas; j++)
    {
        Error_tmp = (Target[j] - this->salida[j]);
        
        this->DeltaH[j] = (1 - this->salida[j]) * this->salida[j] * Error_tmp;
        
        Error_capa = Error_capa + (0.5*Error_tmp*Error_tmp);
        
    }
    return Error_capa;
}

double CapaNeuronal::GetDeltaH(int neurona)
{
    if(neurona>this->cant_neuronas)
    {
        perror("The value exceeds the number of neurons in the layer");
        return 0;
    }
    else
    {
        return( this->DeltaH[neurona]);
    }
}

double CapaNeuronal::GetPeso(int Neurona_capa_siguiente, int Neurona_capa_oculta)
{
    return (this->weights[Neurona_capa_siguiente][Neurona_capa_oculta]);
}

void CapaNeuronal::GetRightWeight()
{
    int i,j;
    for(i = 0; i<this->cant_neuronas; i++)
    {
        for(j = 0; j<this->cant_entradas;j++)
        {
          

            double tmp = Epsilon*this->DeltaH[i]*this->entrada[i];
            if(this->siguiente!=NULL)
            {
                int aux = this->siguiente->GetCantNeuronas();
                for(int h=0; h< aux; h++ )
                {
                    tmp *= this->siguiente->GetPeso(h, i);
                }

            }

            this->weights[i][j] = this->weights[i][j] + tmp + (this->Momento*this->dweights[i][j])  ;
            this->dweights[i][j] = Epsilon*this->DeltaH[i]*this->entrada[i];
        }
        this->Umbral[i] += this->Epsilon*this->DeltaH[i];
    }

}

void CapaNeuronal::SetEntrada(double *In)
{
    if(this->anterior==NULL)
    {
        int i;
        for(i=0; i<this->cant_neuronas;i++)
        {
            this->entrada[i] = In[i];
            
        }
    }

}


void CapaNeuronal::LoadLayer(FILE *archivo)
{
    if(this->anterior!=NULL)
    {
        double tmp;
        int i,j;
        for(i=0; i<cant_neuronas;i++)
        {
            for(j=0; j<cant_entradas; j++)
            {
                while(fscanf(archivo, "%*s %lf", &tmp)==0);
                this->weights[i][j] = tmp;
                
            }
            while(fscanf(archivo, "%*s %lf", &this->Umbral[i])==0);
          
        }
    }
}

void CapaNeuronal::SaveLayer(FILE *archivo)
{

    int i,j;
    for(i=0; i<cant_neuronas;i++)
    {
        for(j=0; j<cant_entradas; j++)
        {
            fprintf(archivo, "Weight(%d,%d): ", i,j);
            fprintf(archivo, "%lf \n", this->weights[i][j]);
        }
        fprintf(archivo, "Layer(%d): %lf \n",i, this->Umbral[i]);
    }
}


