#include "include/RedNeuronal.h"


int Menu(void);


int main(int argc, char **argv)
{
    int opc;
    RedNeuronal *red;
    red = NULL;
    char aux_red[32]="file/red.dat";
    char aux_train[32]="file/train.dat";
    char aux_run[32]="file/run.dat";
    char aux_save[32];
    do
    {
        opc = Menu();

        switch(opc)
        {
            case 1:
            if(red==NULL)
            {
                cin.width(31);
                red = new RedNeuronal(aux_red);
                cout<<"The network is loaded"<<endl;
            }
            else
            {
                cout<<"First remove the existing network"<<endl;
            }
            break;
            case 2:
            if(red!=NULL)
            {
                int a = red->TrainNetwork(aux_train);
                cout<<"The network has been tested in:"<<a<<" times"<<endl;
            }
            else
            {
                cout<<"Create the network first"<<endl;
            }
            break;
            case 3:
            if(red!=NULL)
            {
                red->RunNetwork(aux_run);
            }
            else
            {
                cout<<"Create the network first"<<endl;
            }
            break;
            case 4:
            cout<<"Enter the file you want to save the network currently: ";
            cin.width(31);
            cin>>aux_save;
            if(red!=NULL)
            {
                red->SaveNetwork(aux_save);
            }
            else
            {
                cout<<"Create the network first"<<endl;
            }
            break;
            case 5:
            case 0:
            if(red!=NULL)
            {
            delete red;
            }
            break;
            default:
            cout<<"Enter a correct choice ...."<<endl;
        }


    }while(opc!=0);


    return 0;
}

int Menu(void)
{
    
    cout<< "\tEnter a choice:"<<endl;
    cout<< "\t\t1 - Load the network"<<endl;
    cout<< "\t\t2 - Test the network"<<endl;
    cout<< "\t\t3 - Execute"<<endl;
    cout<< "\t\t4 - Save the network"<<endl;
    cout<< "\t\t5 - Remove the existing network"<<endl;
    cout<< "\t\t0 - Exit"<<endl;
    int aux;
    cin>>aux;
    return aux;
}
