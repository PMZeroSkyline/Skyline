#ifndef Z_THREAD_H
#define Z_THREAD_H
#include "core.h"

class z_thread
{
public:
    #ifdef __APPLE__
        pthread_t t;
    #else
        thread t;
    #endif

    void create(void *(*p)(void *), void *arg)
    {
        #ifdef __APPLE__
            pthread_create(&t, NULL, p, arg);
        #else
            t = thread(p, arg);
        #endif
    }
    void join()
    {
        #ifdef __APPLE__
            pthread_join(t, NULL);
        #else
            t.join();
        #endif
    }
};
#endif
