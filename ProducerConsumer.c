#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREAD_NUM 8

int buffer[5]; 
int count = 0;  

pthread_mutex_t mutex;
sem_t semEmpty;
sem_t semFull;

/*
* Creates a product(number) and puts it in the buffer
*/
void* produce(void* arg)
{
	while (1)
	{
		int product = rand(); // Generate a random number as the product

		sem_wait(&semEmpty);
		pthread_mutex_lock(&mutex);

		buffer[count] = product;
		count++;

		pthread_mutex_unlock(&mutex);
		sem_post(&semFull);
	}
}

/*
* Takes a product from the buffer and prints(consumes) it
*/
void* consume(void* arg)
{
	while (1)
	{
		int product = -1;

		sem_wait(&semFull);
		pthread_mutex_lock(&mutex);

		product = buffer[count-1];
		count--;

		pthread_mutex_unlock(&mutex);
		sem_post(&semEmpty);

		printf("Consumed %d\n", product);
	}
}

int main()
{
	srand(time(NULL));
	// Create threads - half consumers half producers
	pthread_t threads[THREAD_NUM];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&semEmpty, 0, 5); // 5 Empty slots in the beginning
	sem_init(&semFull, 0, 0); // 0 Full slots in the beginning

	for (int i = 0; i < THREAD_NUM; i++)
	{
		if (i%2 == 0)
		{
			if (pthread_create(&threads[i], NULL, &produce, NULL) != 0)
			{
				perror("Failed to create thread");
			}
			printf("Created Producer\n");
		} else {
			if (pthread_create(&threads[i], NULL, &consume, NULL) != 0)
			{
				perror("Failed to create thread");
			}
			printf("Created Consumer\n");
		}
	}

	// Join the threads
	for (int i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			perror("Failed to join thread");
		}
	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&semEmpty);
	sem_destroy(&semFull);

	exit(0);
}
