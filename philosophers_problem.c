#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t footman;                     
sem_t forks[5];             


void eat(int filozof){
	int eatTime = 3;
	printf("\nPhilosopher %d is eating",filozof+1);               
	sleep(eatTime);
	printf("\nPhilosopher %d has finished eating",filozof+1);
}

void* basicPhilLoop(void* arg){
	int filozof=*(int *)arg;
	sem_wait(&footman);                                           
	
	printf("\nPhilosopher %d has entered room",filozof+1);                
	int thinkTime = 3;
	printf("\nPhilosopher %d will think for %d seconds", filozof+1, thinkTime);

	sleep(thinkTime);
	sem_wait(&forks[filozof]);                               
	sem_wait(&forks[(filozof+1)%5]);                         
		
	eat(filozof);                   

	sem_post(&forks[(filozof+1)%5]);                         
	sem_post(&forks[filozof]);
	sem_post(&footman);                                           
	
	free(arg);
}

int main(){
	int i;
	pthread_t th[5];
	
	sem_init(&footman,0,4);                                     
	
	for(i=0;i<5;i++)
		sem_init(&forks[i],0,1);                         
	
	for(i=0;i<5;i++){
		int *noPhil=malloc(sizeof(int));
		*noPhil=i;
		pthread_create(&th[i],NULL,basicPhilLoop,noPhil);       
	}
	for(i=0;i<5;i++)
		pthread_join(th[i],NULL);                          

	sem_destroy(&footman);

	for(i=0;i<5;i++)
		sem_destroy(&forks[i]);

}
