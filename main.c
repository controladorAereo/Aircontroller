#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include<windows.h>

#define N 5
#define N1 15
typedef struct{
    float value;
    int type;
}Tship;


// recurso critico modulo controllador aereo
Tship TvectorPista[2],TvectorPrueba[N],TvectorHangar[N];
int Ind,cont,cont2,band;
float aux;
////////////////////////////////////////////

/////////semaforos///////////
sem_t mutex,full,empty,ship;
/////////////////////////////

void Initializer_Variables();

int FinderShipMaxPriority();
void SetShip(int i);

void *AirController(void *arg){

    while(!band){

       // sem_wait(&ship);
        sem_wait(&empty);
        sem_wait(&mutex);

            SetShip(FinderShipMaxPriority());

        sem_post(&mutex);
        sem_post(&full);

    }//end while

      return 0;
}//end Aircontroller

void *PriorityShip(void *arg){

    while(!band){
        sem_wait(&full);
        sem_wait(&mutex);
            if(TvectorPista[0].type!=0 ){
                printf("ship wich have to go the the hangar -> [%i][%f] cola 1\n",TvectorPista[0].type,TvectorPista[0].value);
                TvectorPista[0].type=0;
                TvectorPista[0].value=0;
                aux=-1;
                //Sleep(1000);
                cont2++;
            }//end if
            if(TvectorPista[1].type!=0){
                printf("ship wich have to go the the hangar -> [%i][%f] cola 2\n",TvectorPista[1].type,TvectorPista[1].value);
                if(TvectorPista[1].value*100>=300){
                    aux=-1;
                }//end if
                TvectorPista[1].type=0;
                TvectorPista[1].value=0;
               // Sleep(1000);
                cont2++;
            }//end if

            if(cont2==N1){
                band=1;
            }//end if
        sem_post(&mutex);
        sem_post(&empty);


    }//end while
    return 0;
}//end*PriorityShip

void *ShipGenerator(void *arg){

    srand(time(NULL));
    while(!band){

        sem_wait(&mutex);
            if(Ind<N && TvectorPrueba[Ind].type==0 ){
                TvectorPrueba[Ind].type=rand() % 3+1;
                TvectorPrueba[Ind].value=(rand() % 500+1);
                TvectorPrueba[Ind].value=TvectorPrueba[Ind].value/100;
                printf("%f carga\n",TvectorPrueba[Ind].value );

                Ind++;
            }//end if
            if(Ind==N){
                Ind=0;
            }
        sem_post(&mutex);
       // sem_post(&ship);
    }//end while
    return 0;
}//end void *ShipGenerator(){}//end

int main(){

    pthread_t thread,thread2,thread3;


    sem_init(&mutex,0,1);
    sem_init(&full,0,0);
    sem_init(&empty,0,2);
    sem_init(&ship,0,0);


    pthread_create(&thread,NULL,AirController,NULL);
    pthread_create(&thread2,NULL,PriorityShip,NULL);
    pthread_create(&thread3,NULL,ShipGenerator,NULL);

    pthread_join(thread,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);

    return 0;

}//end main

void Initializer_Variables(){

    TvectorPista[0].type=0;
    TvectorPista[0].value=0;
    TvectorPista[1].type=0;
    TvectorPista[1].value=0;
    Ind=0;
    aux=-1;
    cont=0;
    cont2=1;
    band=0;

}//end void Initializer_Variables();

int FinderShipMaxPriority(){

    int pos=-1,i=0;
    while(i<N){
        if(TvectorPrueba[i].type>0 && TvectorPrueba[i].value*100>300){
            pos=i;
            aux=TvectorPrueba[i].value;
        }//end if
        i++;
    }//end while

    if(pos<0){
        i=0;
        while(i<N){
            if(TvectorPrueba[i].type>0){
                pos=i;
                aux=TvectorPrueba[i].value;
            }//end if
        i++;
        }//end while
    }//end if
    return pos;

}//end void FinderShipMaxPriority();

void SetShip(int i){

    if(TvectorPista[0].type==0  && i>-1 &&  TvectorPrueba[i].value*100>=300){
        TvectorPista[0].value=TvectorPrueba[i].value;
        TvectorPista[0].type=TvectorPrueba[i].type;
        TvectorPrueba[i].value=0;
        TvectorPrueba[i].type=0;
    }else{
        if(TvectorPista[0].type==0 && i>-1 && TvectorPrueba[i].value*100>=100 && TvectorPrueba[i].value*100<300){
            TvectorPista[0].value=TvectorPrueba[i].value;
            TvectorPista[0].type=TvectorPrueba[i].type;
            TvectorPrueba[i].value=0;
            TvectorPrueba[i].type=0;
        }else{
            if(TvectorPista[1].type==0  && i>-1 && TvectorPrueba[i].value*100<100){
                TvectorPista[1].value=TvectorPrueba[i].value;
                TvectorPista[1].type=TvectorPrueba[i].type;
                TvectorPrueba[i].value=0;
                TvectorPrueba[i].type=0;
            }//end if
        }//end if-else
    }//end if-else

    if(TvectorPista[1].type==0 && i>-1 &&  TvectorPrueba[i].value*100>=300){
        TvectorPista[1].value=TvectorPrueba[i].value;
        TvectorPista[1].type=TvectorPrueba[i].type;
        TvectorPrueba[i].value=0;
        TvectorPrueba[i].type=0;
        cont++;
    }//end if
}//end void SetShip();

