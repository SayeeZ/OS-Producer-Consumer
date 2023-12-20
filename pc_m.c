#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

#define BUFFER_SIZE 10
#define MAX_ITEMS 20

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int produced_count = 0, consumed_count = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_full, cond_empty;
int stop_flag = 0; // Flag to control the loop

void *producer(void *param)
{
    int item = 1;
    while (!stop_flag)
    {
        pthread_mutex_lock(&mutex);

        while ((in + 1) % BUFFER_SIZE == out)
        {
            printf("Buffer full. Producer waiting...\n");
            pthread_cond_wait(&cond_empty, &mutex);
        }

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        produced_count++;

        if (produced_count >= MAX_ITEMS)
        {
            stop_flag = 1;
        }

        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);

        printf("Produced: %d\n", item);
        item++;
        sleep(1);
    }
    return NULL;
}

void *consumer(void *param)
{
    int item;
    while (!stop_flag)
    {
        pthread_mutex_lock(&mutex);

        while (in == out)
        {
            printf("Buffer empty. Consumer waiting...\n");
            pthread_cond_wait(&cond_full, &mutex);
        }

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        consumed_count++;

        if (consumed_count >= MAX_ITEMS)
        {
            stop_flag = 1;
        }

        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);

        printf("Consumed: %d\n", item);
        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t prod_thread, cons_thread;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_full, NULL);
    pthread_cond_init(&cond_empty, NULL);

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Join threads
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Cleanup mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}
