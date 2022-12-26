#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int a = 0;
pthread_mutex_t m;

void* r()
{
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&m);
        printf("%ld %d\n", pthread_self(), a);
        pthread_mutex_unlock(&m);
        sleep(3);
    }
    return NULL;
}

void* w()
{
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&m);
        a++;
        pthread_mutex_unlock(&m);
        sleep(3);
    }
    return NULL;
}
int main() 
{
    pthread_t t[11];
    pthread_mutex_init(&m, NULL);
    pthread_create(&t[0], NULL, w, NULL);
    for (int i = 1; i < 11; i++) {
        pthread_create(&t[i], NULL, r, NULL);
    }
    pthread_join(t[10], NULL);
    return 0;
}