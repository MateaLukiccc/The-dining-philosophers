#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

sem_t footman;                      //ogranicen broj filozofa u sobi na 4
sem_t forks[5];              // kako je 5 filozofa postoji i 5 stapica za jelo


void eat(int filozof){
	int eatTime = rand() % 3 + 1;
	printf("\nPhilosopher %d is eating",filozof+1);               //posto numerisanje pocinje od 0 filozof+1 oznacava redni broj filozofa poceco od 1 umesto 0
	sleep(eatTime);
	printf("\nPhilosopher %d has finished eating",filozof+1);
}

void* basicPhilLoop(void* arg){
	int filozof=*(int *)arg;
	sem_wait(&footman);                                           //svakim ulaskom filozofa semafor room gubi 1 resurs(ima 4 na pocetku i kraju)
	
	printf("\nPhilosopher %d has entered room",filozof+1);                //razmislja pa podize stapice
	int sleepTime = rand() % 3 + 1;
	printf("\nPhilosopher %d will think for %d seconds", filozof+1, sleepTime);

	sleep(sleepTime);
	sem_wait(&forks[filozof]);                                //filozof uzima levi stapic
	sem_wait(&forks[(filozof+1)%5]);                          //filozof uzima desni stapic i onemogucava drugima da ih uzimaju(vrednost semafora za svaki stapic je 1)
		
	eat(filozof);                   

	sem_post(&forks[(filozof+1)%5]);                          //filozof zavrsava jelo i ostavlja levi i desni stapic
	sem_post(&forks[filozof]);
	sem_post(&footman);                                           //filozof izlazi iz sobe popvecavajuciu semafor za 1 omogucavajuci jos jednom(poslednjem filozofu da udje u nju)
	
	free(arg);
}

int main(){
	int i;
	pthread_t th[5];
	
	sem_init(&footman,0,4);                                      //pocetna vrednost semafora je 4 da bi uslo tacno 4 filozofa
	
	for(i=0;i<5;i++)
		sem_init(&forks[i],0,1);                          //kreira se 5 semafora za stapice
	
	for(i=0;i<5;i++){
		int *noPhil=malloc(sizeof(int));
		*noPhil=i;
		pthread_create(&th[i],NULL,basicPhilLoop,noPhil);        //kreira se 5 niti koji koriste funkciju filozof i salju redni broj filozofa poceci od 0
	}
	for(i=0;i<5;i++)
		pthread_join(th[i],NULL);                           //unistavanje kreiranih niti i semafora

	sem_destroy(&footman);

	for(i=0;i<5;i++)
		sem_destroy(&forks[i]);

}