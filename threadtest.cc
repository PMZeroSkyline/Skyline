#include <iostream>
#include <thread>
#include <string>
#include <vector>

using namespace std;

void doSomething(int id)
{
    cout << id << "\n";
}

void spawnThreads(int n)
{
    vector<thread> threads(n);
    for (size_t i = 0; i < n; i++)
    {
        threads[i] = thread(doSomething, i + 1);
    }
    for (auto &t : threads)
    {
        t.join();
    }
    
}
int main()
{
    
    spawnThreads(10);
    return 0;
}