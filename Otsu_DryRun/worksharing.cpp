#include <string>
#include <stdio.h>


double data[N];
int i;
#pragma omp parallel shared(data)
{
#pragma omp single private(i)
    {
        for (i = 0, i < N; i++)
        {
#pragma omp task firstprivate(i) shared(data))
            {
                do_work(data, i);
            }
        }
    }
}