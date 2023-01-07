#include <iostream>
#include <thread>
#include <string>
#include <vector>

using namespace std;

void download(string downloadName)
{
    for (size_t i = 0; i < 100; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(30));
        cout << downloadName + to_string(i) << endl;
    }
}

int main()
{
    thread t1(download, "download 1 : ");
    thread t2(download, "download 2 : ");
    t1.join();
    t2.join();
    return 0;
}