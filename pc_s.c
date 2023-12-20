#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

#define BUFFER_SIZE 10
#define MAX_ITEMS 20

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int produced_count = 0, consumed_count = 0;
sem_t empty, full;
int stop_flag = 0; // Flag to control the loop

void *producer(void *param)
{
    int item = 1;
    while (!stop_flag)
    {
        sem_wait(&empty); // Decrease the empty slot count

        if ((in + 1) % BUFFER_SIZE == out)
        {
            printf("Buffer full. Producer waiting...\n");
        }

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        produced_count++;

        if (produced_count >= MAX_ITEMS)
        {
            stop_flag = 1; // Set flag to stop the loop
        }

        sem_post(&full); // Increase the full slot count

        printf("Produced: %d\n", item);
        item++;   // Increment the item for the next production
        sleep(1); // Sleep for 1 second
    }
    return NULL;
}

void *consumer(void *param)
{
    int item;
    while (!stop_flag)
    {
        sem_wait(&full); // Decrease the full slot count

        if (in == out)
        {
            printf("Buffer empty. Consumer waiting...\n");
        }

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        consumed_count++;

        if (consumed_count >= MAX_ITEMS)
        {
            stop_flag = 1; // Set flag to stop the loop
        }

        sem_post(&empty); // Increase the empty slot count

        printf("Consumed: %d\n", item);
        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t prod_thread, cons_thread;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads (optional, for this example, we will leave the threads running)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Cleanup semaphores
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}