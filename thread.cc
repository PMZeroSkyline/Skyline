#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

void task(int count)
{
    int sum = 0;
    for (int i = 0; i < count; i++)
    {  
        sum += i;
    }
    cout << "\nthread id = " << this_thread::get_id() << "\tsum = " << sum << endl;
}

int main(int argc, char **argv)
{
    int thread_count = thread::hardware_concurrency();

    cout << "\nthread_count = " << thread_count << endl;
    
    int sum_loop_count = 100000000;

    clock_t beg = clock();

    vector<thread> thread_vec;
    for (int i = 0; i < thread_count; i++)
    {
        thread_vec.push_back(thread(task, (int)(sum_loop_count/thread_count)));
    }
    for (int i = 0; i < thread_count; i++)
    {
        thread_vec[i].join();
    }
    
    
    cout << "\n" << (double)(clock()-beg)/CLOCKS_PER_SEC << endl;
}