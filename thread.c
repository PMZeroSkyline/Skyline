#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define A 100000000
#define N 8

const int SUM = A / N;

void *func(void *arg)
{
    int sum = SUM;
    for (int i = 0; i < sum; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            int num = j;
        }
    }
    return NULL;    
}
int main()
{
    clock_t beg = clock();
    pthread_t tid[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&tid[i], NULL, func, NULL);
    }
    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }
    clock_t end = clock();

    double t = (double)(end - beg) / CLOCKS_PER_SEC;
    
    printf("time , %f", t);
}