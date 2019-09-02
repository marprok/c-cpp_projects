#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /*For sleep*/

/*
  Consumer producer thread interaction.
*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bufferFull = PTHREAD_COND_INITIALIZER, bufferEmpty = PTHREAD_COND_INITIALIZER;
typedef struct
{
    int* buf;
    int size;
    int next;
} buffer_t;

void* producer(void* data)
{
    // pthread_detach(pthread_self());
    buffer_t* buffer = (buffer_t*)data;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (buffer->next >= buffer->size - 1)
        {
            printf("PRODUCER: buffer full\n");
            pthread_cond_wait(&bufferFull, &mutex);
        }

        buffer->buf[++buffer->next] = rand() % 5 + 1;
        printf("PRODUCER: produced a packet with data %i\n", buffer->buf[buffer->next]);
        pthread_cond_signal(&bufferEmpty);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }

    return NULL;
}

void* consumer(void* data)
{
    // pthread_detach(pthread_self());
    buffer_t* buffer = (buffer_t*)data;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (buffer->next < 0)
        {
            printf("CONSUMER: buffer empty\n");
            pthread_cond_wait(&bufferEmpty, &mutex);
        }
        int packet = buffer->buf[buffer->next--];
        pthread_cond_signal(&bufferFull);
        pthread_mutex_unlock(&mutex);
        printf("CONSUMER: consumed packet with data %i\n", packet);
        sleep(packet); /*Consume packet*/
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    int size = 10;
    int* buffer = malloc(size * sizeof(int));
    buffer_t buf = { buffer, size, -1 };

    memset(buffer, 0, size * sizeof(int));

    pthread_t cons, prod;

    int flag;

    flag = pthread_create(&cons, NULL, consumer, (void*)&buf);
    if (flag)
    {
        fprintf(stderr, "ERROR: Could not create the consumer thread!\n");
        exit(EXIT_FAILURE);
    }

    flag = pthread_create(&prod, NULL, producer, (void*)&buf);
    if (flag)
    {
        fprintf(stderr, "ERROR: Could not create the producer thread!\n");
        exit(EXIT_FAILURE);
    }
    printf("Threads started\n");

    // pthread_join(cons, NULL);
    // pthread_join(prod, NULL);
    sleep(10);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&bufferFull);
    pthread_cond_destroy(&bufferEmpty);
    free(buffer);
    // pthread_detach(pthread_self());
    return 0;
}
