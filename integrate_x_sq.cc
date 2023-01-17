#include "core.h"

int main()
{
    int N = 1000000;
    double sum = 0;

    for (int i = 0; i < N; i++)
    {
        double x = random_double(0, 2);
        sum += x*x;
    }
    cout << std::fixed << setprecision(12);
    cout << "I = " << 2 * sum/N << '\n';
    
    return 0;
}