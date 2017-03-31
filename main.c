#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include<windows.h>

#define N 5
typedef struct{
    float value;
    int type;
}Tship;


// recurso critico modulo controllador aereo
Tship TvectorPista[2],TvectorPrueba[N];
int Ind=0,Ind2=0;
////////////////////////////////////////////

sem_t mutex,full,empty,ship;

void Initializer_Variables();



void *AirController(void *arg){

    while(1){

       // sem_wait(&ship);
        sem_wait(&empty);
        sem_wait(&mutex);

            if(TvectorPista[0].type==0 && Ind2<Ind && TvectorPrueba[Ind2].value*100>=300){
                TvectorPista[0].value=TvectorPrueba[Ind2].value;
                TvectorPista[0].type=TvectorPrueba[Ind2].type;
                Ind2++;
            }else{
                if(TvectorPista[0].type==0 && Ind2<Ind && TvectorPrueba[Ind2].value*100>=100 && TvectorPrueba[Ind2].value*100<300){
                    TvectorPista[0].value=TvectorPrueba[Ind2].value;
                    TvectorPista[0].type=TvectorPrueba[Ind2].type;
                    Ind2++;
                }else{
                    if(TvectorPista[1].type==0 && Ind2<Ind && TvectorPrueba[Ind2].value*100<100){
                        TvectorPista[1].value=TvectorPrueba[Ind2].value;
                        TvectorPista[1].type=TvectorPrueba[Ind2].type;
                        Ind2++;
                    }//end if
                }//end if-else
            }//end if-else

            if(Ind2==N){
                Ind2=0;
                Ind=0;
            }//end if

        sem_post(&mutex);
        sem_post(&full);

    }//end while

      return 0;
}//end Aircontroller

void *PriorityShip(void *arg){

    while(1){

        sem_wait(&full);
        sem_wait(&mutex);

            if(TvectorPista[0].type!=0 ){
                printf("ship wich have to go the the hangar -> [%i][%f] cola 1\n",TvectorPista[0].type,TvectorPista[0].value);
                TvectorPista[0].type=0;
                TvectorPista[0].value=0;
                Sleep(1000);
            }//end if
            if(TvectorPista[1].type!=0){
                printf("ship wich have to go the the hangar -> [%i][%f] cola 2\n",TvectorPista[1].type,TvectorPista[1].value);
                TvectorPista[1].type=0;
                TvectorPista[1].value=0;
                Sleep(1000);
            }//end if

        sem_post(&mutex);
        sem_post(&empty);


    }//end while
    return 0;
}//end*PriorityShip

void *ShipGenerator(void *arg){

    srand(time(NULL));
    while(1){

        sem_wait(&mutex);
            if(Ind<N){
                TvectorPrueba[Ind].type=rand() % 3+1;
                TvectorPrueba[Ind].value=(rand() % 500+1);
                TvectorPrueba[Ind].value=TvectorPrueba[Ind].value/100;
                printf("%f carga\n",TvectorPrueba[Ind].value );
                getchar();
                Ind++;
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


}//end void Initializer_Variables();

