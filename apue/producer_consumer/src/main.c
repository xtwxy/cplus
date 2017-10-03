#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NOPRINT
  #define PRINTF(format, ... )
#else
  #define PRINTF printf
#endif

struct shared_queue {
    pthread_mutex_t lock;
    pthread_cond_t full;
    pthread_cond_t empty;
    size_t produced;
    size_t consumed;
    size_t max_count;
    size_t capacity;
};

static pthread_once_t queue_init_once = PTHREAD_ONCE_INIT;
static pthread_once_t queue_destroy_once = PTHREAD_ONCE_INIT;
static struct shared_queue queue;

void init_queue() {
    pthread_mutex_init(&queue.lock, NULL);
    pthread_cond_init(&queue.full, NULL);
    pthread_cond_init(&queue.empty, NULL);
    PRINTF("lock and shared_queue initialized.\n");
}

void destroy_queue() {
    pthread_mutex_destroy(&queue.lock);
    pthread_cond_destroy(&queue.full);
    pthread_cond_destroy(&queue.empty);
    PRINTF("lock and shared_queue destroyed.\n");
}

void* producer_thread(void *p) {
    struct shared_queue *q = (struct shared_queue *)p;
    for(; q->produced != q->max_count; ) {
        pthread_mutex_lock(&(q->lock));
        if((q->produced - q->consumed) < q->capacity) {
            q->produced++;
            PRINTF("produced: %lu\n", q->produced);
            PRINTF("producer(produced, consumed) => (%lu, %lu)\n", q->produced, q->consumed);
            pthread_cond_signal(&(q->empty));
        } else {
            PRINTF("full: wait for consuming.\n");
            pthread_cond_wait(&(q->full), &(q->lock));
        }
        pthread_mutex_unlock(&(q->lock));
    }
    return p;
}

void* consumer_thread(void *p) {
    struct shared_queue *q = (struct shared_queue *)p;
    for(; q->consumed != q->max_count; ) {
        pthread_mutex_lock(&(q->lock));
        if(q->produced != q->consumed) {
            q->consumed++;
            PRINTF("consumed: %lu\n", q->consumed);
            PRINTF("consumer(produced, consumed) => (%lu, %lu)\n", q->produced, q->consumed);
            pthread_cond_signal(&(q->full));
        } else {
            PRINTF("empty: wait for producing.\n");
            pthread_cond_wait(&(q->empty), &(q->lock));
        }
        pthread_mutex_unlock(&(q->lock));
    }
    return p;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        PRINTF("Usage: %s <queue capacity> <max count to produce>\n", argv[0]);
        return EXIT_FAILURE;
    }

    memset(&queue, 0, sizeof(queue));

    pthread_once(&queue_init_once, init_queue);
    queue.capacity = atoi(argv[1]);
    queue.max_count = atoi(argv[2]);

    pthread_attr_t attr;
    void *result;
    pthread_t producer;
    pthread_t consumer;
    pthread_attr_init(&attr);

    pthread_create(&producer, &attr, &producer_thread, &queue);
    pthread_create(&consumer, &attr, &consumer_thread, &queue);
    
    pthread_join(producer, &result);
    pthread_join(consumer, &result);
    pthread_attr_destroy(&attr);

    pthread_once(&queue_destroy_once, destroy_queue);

    return EXIT_SUCCESS;
}

