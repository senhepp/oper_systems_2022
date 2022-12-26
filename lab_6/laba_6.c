#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int a = 0;
pthread_mutex_t m;
pthread_cond_t c;

void* r()
{
    for (int i = 0; i < 5; i++) {
        sleep(2);
        pthread_mutex_lock(&m);
        pthread_cond_wait(&c, &m);
        printf("%ld %d\n", pthread_self(), a);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

void* w()
{
    for (int i = 0; i < 5; i++) {
        sleep(3);
        pthread_mutex_lock(&m);
        a++;
        pthread_cond_broadcast(&c);
        pthread_mutex_unlock(&m);
    }
    pthread_cond_broadcast(&c);
    return NULL;
}
int main()
{
    pthread_cond_init(&c, NULL);
    pthread_t t[11];
    pthread_mutex_init(&m, NULL);
    pthread_create(&t[0], NULL, w, NULL);
    for (int i = 1; i < 11; i++) {
        pthread_create(&t[i], NULL, r, NULL);
    }
    pthread_join(t[10], NULL);
    return 0;
}