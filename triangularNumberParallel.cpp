#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>

#define ARRAYLENGTH 10000
#define THREADS 2

using namespace std;

int main()
{
	int A[ARRAYLENGTH], B[ARRAYLENGTH], index;
	double currentTime, totalTime;

	currentTime = omp_get_wtime();

	#pragma omp parallel num_threads(THREADS)
	{
		#pragma omp for
			for(index = 1; index <= ARRAYLENGTH; index++)
			{
				A[index - 1] = index;
			}
	}

	#pragma omp parallel num_threads(THREADS)
		B[0] = A[0];

	#pragma omp parallel num_threads(THREADS)
	{
		#pragma omp for private(index)
			for(index = 1; index < ARRAYLENGTH; index++)
			{
				B[index] = B[index - 1] + A[index];
			}
	}

	totalTime = omp_get_wtime() - currentTime;

	printf("Total Time: %f\n", totalTime);

	for(index = 0; index < ARRAYLENGTH; index++)
	{
        cout << "B[" << index << "] =  " << B[index] << "\n";
	}

	return 0;
}