#include "../include/RedNeuronal.h"


RedNeuronal::RedNeuronal(int n_capas, int *n_neur)
{
    //ctor
    this->numero_de_capas = n_capas;
    this->n_neuronas = new int[n_capas];
    int i;
    for(i=0; i<n_capas;i++)
    {
        this->n_neuronas[i] = n_neur[i];
    }
    this->CreateNetwork(NULL);

}

RedNeuronal::RedNeuronal(char *archivo)
{
    this->LoadNetwork(archivo);


}

RedNeuronal::~RedNeuronal()
{
    delete [] Input;
    delete [] Output;
    delete [] Target;
    delete [] capa0;
    delete [] n_neuronas;
}


void RedNeuronal::FeedPropagate()
{
    int i;
    for(i=0; i<this->numero_de_capas; i++)
    {
        this->capa0[i].ProcessOutput();
    }
    i--;
    for(int j = 0; j<this->n_neuronas[i];j++)
    {
        this->Output[j] = this->capa0[i].GetSalida(j);
        cout<<endl;
       
    }

}


void RedNeuronal::BackPropagate()
{
    
    this->FeedPropagate();
    this->ComputeTotalError();
}


void RedNeuronal::ComputeTotalError()
{
    int nc1 = this->numero_de_capas -1;
    this->ErrorTotal = this->capa0[nc1].ComputeError(this->Target);

    for(--nc1; nc1>0; nc1--)
    {
        this->capa0[nc1].ComputeError();
    }
       
    for(nc1=1; nc1<this->numero_de_capas;nc1++)
    {
            this->capa0[nc1].GetRightWeight();
    }

}



void RedNeuronal::CreateNetwork(FILE *archivo)
{
    int i= 0;

    this->capa0 = new CapaNeuronal[this->numero_de_capas];
    this->capa0[0].New(n_neuronas[0], NULL, NULL);
    this->cant_entradas = n_neuronas[i];
    int n;
    for (i = 1; i<this->numero_de_capas; i++)
    {
        
        this->capa0[i].New((int)(n_neuronas[i]),&this->capa0[i-1],archivo);
    }
    i--;
    this->cant_salidas = n_neuronas[i];
    this->Target = new double[cant_salidas];
    this->Input = new double[cant_entradas];
    this->Output = new double[cant_salidas];

}


void RedNeuronal::SaveNetwork(char *file_name)
{
    FILE *f;
    f = fopen(file_name, "w");
    if(!f)
    {
        cout <<"Error loading network !!"<<endl;
        perror(file_name);
        exit(-2);
    }
    else
    {
        
        fprintf(f, "Number of layers: %d\n", this->numero_de_capas);
        int i;
        for(i=0; i<numero_de_capas;i++)
        {
            while(fprintf(f, "Layer neurons%d: %d\n",i, this->n_neuronas[i])==0);
        }
        i = 1;
        fprintf(f, "Load weights? cero=NO one=YES: %d\n", i);
        if(i==1)
        {
            for(;i<numero_de_capas; i++)
            {
                fprintf(f, "Layer weights%d:\n", i);
                this->capa0[i].SaveLayer(f);
                fprintf(f, "\n");
            }
        }
        fclose(f);
    }

}

void RedNeuronal::LoadNetwork(char *file_name)
{
    FILE *f = fopen(file_name, "r");
    if(!f)
    {
        cout <<"Error loading network !!"<<endl;
        perror(file_name);
        exit(-2);
    }
    else
    {
        
        while(fscanf(f, "%*s %d", &this->numero_de_capas)==0);
        int i;
        int estado;
        this->n_neuronas = new int[numero_de_capas];
        for(i=0; i<numero_de_capas;i++)
        {
            while(fscanf(f, "%*s %d", &this->n_neuronas[i])==0);
            

        }
        while(fscanf(f, "%*s %d", &estado)==0);
     
        if(estado==0)
        {
            //fcloseall();
            f = NULL;
        }
        this->CreateNetwork(f);
    }


}

long int RedNeuronal::TrainNetwork(char *archivito)
{
    this->ErrorTotal = 0;
    long int epocas = 0;
    int corregido, i, j;
    this->PrepareTraining(archivito);
    FILE *f;
    f = fopen("error.dat", "w");
    if(!f)
    {
        perror("Error opening file\n");
        exit(-1);
    }
    if(n_casos >0 )
    {

        do
        {
            for(i=0; i<n_casos; i++)
            {
                
                this->capa0[0].SetEntrada(patron[i]);
               
                for(int h = 0, j = cant_entradas; h<this->cant_salidas ; h++, j++)
                {
                    this->Target[h] = patron[i][j];
                    
                }
              
                this->Train();
            }
            fprintf(f, "%d %lf \n",epocas, this->ErrorTotal);
            cout<<"Period Number:"<<epocas<<" Total Number of Network Errors: "<<this->ErrorTotal<<endl;
            epocas++;
        }while( epocas < 200);
    }
    delete [] patron;
    return epocas;

}

void RedNeuronal::Train()
{
        this->BackPropagate();
}


void RedNeuronal::PrepareTraining(char *archivo)
{
    FILE *f = fopen(archivo, "r");
    if(!f)
    {
        perror(archivo);
    }
    else{
    int casos, i, j;
    while(fscanf(f, "%*s %d", &casos)==0);
  
    patron = new double*[casos];
    for(i=0; i<casos; i++)
    {
        patron[i] = new double[this->cant_entradas+this->cant_salidas];
        for(j=0; j<(this->cant_entradas+this->cant_salidas);j++)
        {
            while(fscanf(f, "%*s %lf", &patron[i][j])==0);
         
        }
    }
    fclose(f);
   
    this->n_casos = casos;}
}

void RedNeuronal::RunNetwork(char *archivito)
{
    FILE *f = fopen(archivito, "r");
    if(!f)
    {
        perror(archivito);
    }
    else
    {
        int i, j, casos;
        while(fscanf(f, "%*s %d", &casos)==0);
        for(i=0; i<casos; i++)
        {
            for(j=0; j<this->cant_entradas;j++)
            {
                double tmp;
                while(fscanf(f, "%*s %lf", &tmp)==0);
                this->Input[j] = tmp;
              
            }
            this->capa0[0].SetEntrada(this->Input);
            this->FeedPropagate();
            cout<<"Case Number: "<<i<<endl;
            for(j = 0; j<this->cant_salidas; j++)
            {
                cout<<"Network output("<<j<<") :"<<this->Output[j]<<endl;
            }

        }
        fclose(f);
    }
}


double RedNeuronal::GetSalida(int neurona)
{
    if(neurona>this->cant_salidas)
    {
        return 0.0f;
    }
    else
    return(this->Output[neurona]);
}
