/*
 * Author: Travis Weaver
 * University of Miami - CSC 528
 * Compile: icc -openmp parallelMergeSortOMP.cpp -o parallelMergeSortOMP
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;

#define LIST_SIZE 1000000

int numThreads = 4;
int isVerbose = 0;
double startTime = 0.0, endTime = 0.0;

/*
 * Generates random unsorted list of integers using the size of the list as the
 * range of possible numbers
 */

void populateList(int * ptr, int size)
{
	int i, j, tmp;

	for(i = 0; i < size; i++)
	{
		ptr[i] = i;
	}

	for(i = 0; i < size; i++)
	{
		j = rand() % size;
		tmp = ptr[i];
		ptr[i] = ptr[j];
		ptr[j] = tmp;
	}
}

void parallelMerge(int * left, int * right, int sizel, int sizer, int * ptr)
{
	int size = LIST_SIZE;
	int eleThreshold = size / numThreads;

	if(isVerbose)
	{
		printf("\nTesting parallelMerge 1\n");
	}

	if(sizer < eleThreshold || sizel < eleThreshold)
	{
		merge(left, left + sizel, right, right + sizer, ptr);
	}

	else
	{
		int middle = sizel / 2;
		int partition = upper_bound(right, right + sizer, left[middle]) - right;

		#pragma omp task
		{
			parallelMerge(left, right, middle, partition, ptr);
		}

		#pragma omp task
		{
			parallelMerge(left + middle, right + partition, sizel - middle, sizer - partition, ptr + partition + middle);
		}

		#pragma omp taskwait
	}
}

int mergeSort(int * ptr, int * tmp, int size)
{
	if(size < 2)
	{
		sort(ptr, ptr + size);
		return 0;
	}

	int sizel, sizer;
	int arr1, arr2;

	sizel = size / 2 + size % 2;
	sizer = size / 2;

	if(isVerbose)
	{
		printf("\nTesting mergeSort 1\n");
	}

	#pragma omp task
	{
		arr1 = mergeSort(ptr, tmp, sizel);
	}

	#pragma omp task
	{
		arr2 = mergeSort(ptr + sizel, tmp + sizel, sizer);
	}

	#pragma omp taskwait

	if(arr1 == 0)
	{
		parallelMerge(ptr, ptr + sizel, sizel, sizer, tmp);
	}

	if(arr2 == 0)
	{
		parallelMerge(tmp, tmp + sizel, sizel, sizer, ptr);
	}

	return 0;
}

int main()
{
	int n = LIST_SIZE;
	//int list[LIST_SIZE];
	//int tmp[LIST_SIZE];
	vector<int> list(n);
	vector<int> tmp(n);
	int index;

	#pragma omp parallel num_threads(numThreads)
	{
		numThreads = omp_get_num_threads();
	}

	printf("Number of threads = %d\n", numThreads);
	populateList(&list[0], n);

	if(isVerbose == 1)
	{
		printf("Unsorted List: \n");

		for(index = 0; index < n; index++)
		{
			printf("%d ", list[index]);
		}

		printf("\n");
	}

	startTime = omp_get_wtime();
	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp single
		{
			mergeSort(&list[0], &tmp[0], n);
		}
	}
	endTime = omp_get_wtime();
	double totalTime = endTime - startTime;

	printf("Total Runtime: %f seconds\n", totalTime);

	if(isVerbose == 1)
	{
		printf("\nSorted List: \n");

		for(index = 0; index < n; index++)
		{
			printf("%d ", list[index]);
		}

		printf("\n");
	}

	return(EXIT_SUCCESS);
}