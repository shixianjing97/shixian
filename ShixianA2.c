#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
int Buffer_milk[9] = { 0,0,0,0,0,0,0,0,0 };
int Buffer_cheese[4] = { 0,0,0,0 };
int M = 9;
int CS = 4;
int W,R,C,B = 0;

sem_t empty;
sem_t mutex;
sem_t full;
sem_t Empty;
sem_t Mutex;
sem_t Full;

void* MilkProduc(void* arr)//Create 3 milk_producer threads
{
	int* T = (int*)arr;
	for (int i = 0; i < T[1]; i++)//A[1] is the number of itmes
	{//The buffer access (by milk_producer, cheese_producer) is protected by a mutex lock (use a semaphore to implement mutex)
		sem_wait(&empty);
		sem_wait(&mutex);

		Buffer_milk[W] = T[0];//the producer ID
		W++;
		if (W == M)//Produce a bottle of milk and put it in the refrigerator (buffer_milk: size 9)
			W = W % M;

		sem_post(&mutex);
		sem_post(&full);
	}
}
void* CheeseProduc(void* arr)//Create 2 cheese_producer threads
{
	int* T = (int*)arr;
	int ID;
	for (int i = 0; i < T[1]; i++)
	{

		sem_wait(&full);
		sem_wait(&full);
		sem_wait(&full);// If there are less than three milk bottles, the thread waits(use semaphore)
		sem_wait(&mutex);


		ID = 1000 * Buffer_milk[R];

		Buffer_milk[R] = 0;

		R++;
		if (R == M)
			R = R % M;

		ID += 100 * Buffer_milk[R];
		Buffer_milk[R] = 0;

		R++;;
		if (R == M)
			R = R % M;


		ID += 10 * Buffer_milk[R];
		Buffer_milk[R] = 0;

		R++;
		if (R == M)
			R = R % M;

		ID += T[0];

		sem_post(&mutex);
		sem_post(&empty);
		sem_post(&empty);
		sem_post(&empty);

		sem_wait(&Empty);
		sem_wait(&Mutex);


		Buffer_cheese[C] = ID;

		C = C + 1;
		if (C== CS)
			C= C % CS;


		sem_post(&Mutex);
		sem_post(&Full);
	}
}
void* BurgerProduc(void* arr)//Create 1 cheeseburger_producer threads
{
	int N = *(int*)arr;
	int ID;
	for (int i = 0; i < N; i++)
	{
		sem_wait(&Full);
		sem_wait(&Full);//If there are less than two slices of cheese, it waits (use semaphore) 
		sem_wait(&Mutex);

		//Print the cheeseburger ID: concatenation of two cheese slice IDs
		ID = Buffer_cheese[B] * 10000;
		Buffer_cheese[B] = 0;
		B = B + 1;

		if (B == CS)
			B = B % CS;

		ID += Buffer_cheese[B];
		Buffer_cheese[B] = 0;
		B = B + 1;

		if (B == CS)
			B = B % CS;


		printf("CheeseBurger ID: %d\n", ID);

		sem_post(&Mutex);
		sem_post(&Empty);
		sem_post(&Empty);
	}




}
int main()
{
	int Num;
	printf("# of cheeseburgers:");
	scanf("%d", &Num);// it will read a nonnegative integer (# of cheeseburgers.)
	while (Num < 0)// while it is negative
	{
		printf("# of cheeseburgers should be non-negative number,again: \n");
		scanf("%d", &Num);
	}

	// calculates the required numbers of items for each thread. 
	int cheese = Num * 2;
	int milk = cheese * 3;


	//Pass the ID of each producer and the number of items assigned to each thread: IDs of milk producers (1, 2, 3), cheese producers (4, 5) 
	int MT1[2] = { 1, cheese };
	int MT2[2] = { 2, cheese };
	int MT3[2] = { 3, cheese };
	int CT1[2] = { 4, Num };
	int CT2[2] = { 5, Num };

	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 9);
	sem_init(&Mutex, 0, 1);
	sem_init(&Full, 0, 0);
	sem_init(&Empty, 0, 4);
	//pthread_create(&tid, NULL, runner, (void*) args);
	pthread_t M1;//milk 1
	pthread_t M2;
	pthread_t M3;
	pthread_t C1;//cheese 1
	pthread_t C2;
	pthread_t B1;//cheeseburger 1
	pthread_create(&M1, NULL, MilkProduc, (void*)&MT1);
	pthread_create(&M2, NULL, MilkProduc, (void*)&MT2);
	pthread_create(&M3, NULL, MilkProduc, (void*)&MT3);
	pthread_create(&C1, NULL, CheeseProduc, (void*)&CT1);
	pthread_create(&C2, NULL, CheeseProduc, (void*)&CT2);
	pthread_create(&B1, NULL, BurgerProduc, (void*)&Num);

	//The main program waits for the completion of threads using: pthread_join(&tid, NULL) 
	pthread_join(M1, NULL);
	pthread_join(M2, NULL);
	pthread_join(M3, NULL);
	pthread_join(C1, NULL);
	pthread_join(C2, NULL);
	pthread_join(B1, NULL);
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
	sem_destroy(&Mutex);
	sem_destroy(&Full);
	sem_destroy(&Empty);
	exit(0);
}
