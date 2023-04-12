#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int arg,char *arc[]){

char comando[100];
int **pipe_fd;
int i,j,status,n;
pid_t padre,child;

int p,p_alt;

padre=getpid();

n=atoi(arc[1]);

pipe_fd=(int **)malloc(sizeof(int*)*2*n);//Le doy un espacio de memoria a un puntero doble
                                 //Que me servira para guardar los pipes de todos los procesos

for(int i=0;i<2*n;i++){
    pipe_fd[i]=(int *)malloc(sizeof(int)*2);//A cada elemento del arreglo le otorga un arreglo int
                                      // tamaño 2 para poder almancenar los valores fd de los pipes

    pipe(pipe_fd[i]);
}

for(i=0;i<2;i++){

    if( !(fork()) ){//Si es hijo
        for(j=0;j<n;j++){
            if( (child=fork())<=0 ){//Creamos n hijos

                srand(getpid());//Obtenemos una seed aleatoria;
                p=rand();

                fprintf(stderr,"valor de i= %d , valor de j= %d pid del proceso %d numero %d \n",
                                    i,j,getpid(),p);

                int lec=i*n+j;
                int esc=(!i)*n+j;

                close(pipe_fd[lec][1]);//Ciero el de escritura, solo sirve para leer
                close(pipe_fd[esc][0]);//Cierro el de lectura, solo sirve para escribir

                sleep(1);

                write(pipe_fd[esc][1],&p,sizeof(int));
                read(pipe_fd[lec][0],&p_alt,sizeof(int));
                

                fprintf(stderr,"valor de i= %d , valor de j= %d pid del proceso %d numero %d otro numero aleatorio %d \n",i,j,getpid(),p,p_alt);
                                    

                if(i==1 && j==n-1){
                    sprintf(comando,"pstree -s -p %d",padre);
                    system(comando);
                    
                }

                pause();

            }
           
        }
        wait(&status);
    }
    //wait(&status);//Si es padre esperamos a que todos los hijos terminen para tomar la foto




}
wait(&status);





return 0;
}
