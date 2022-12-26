#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int a = 0;
pthread_rwlock_t l;

void* r()
{
    for (int i = 0; i < 5; i++) {
        pthread_rwlock_rdlock(&l);
        printf("%ld %d\n", pthread_self(), a);
        pthread_rwlock_unlock(&l);
        sleep(3);
    }
    return NULL;
}

void* w()
{
    for (int i = 0; i < 5; i++) {
        pthread_rwlock_wrlock(&l);
        a++;
        pthread_rwlock_unlock(&l);
        sleep(3);
    }
    return NULL;
}
int main()
{
    pthread_t t[11];
    pthread_rwlock_init(&l, NULL);
    pthread_create(&t[0], NULL, w, NULL);
    for (int i = 1; i < 11; i++) {
        pthread_create(&t[i], NULL, r, NULL);
    }
    pthread_join(t[10], NULL);
    return 0;
}