/*
 * Author: Travis Weaver
 * University of Miami - CSC 528
 * Compile: mpicc filename
 * Run: mpirun -np <# of threads> ./filename
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <iostream>

using namespace std;

#define LIST_SIZE 100

int isVerbose = 1;
int tasks, rank;
double startTime = 0.0, endTime = 0.0;

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

int min(int a, int b) 
{
	if (a <= b) return a;
	return b;
}

void merge(int * list, int * tmp, int sizel, int sizer, int sizem)
{
	int index, i, j;

	i = sizel;
	j = sizer;

	if(isVerbose)
	{
		printf("\nTesting merge 1\n");
	}

	for(index = sizel; index < sizem; index++)
	{
		if((i < sizer) && (j >= sizem || list[i] <= list[j]))
		{
			tmp[index] = list[i];
			i += 1;
		}

		else
		{
			tmp[index] = list[j];
			j += 1;
		}
	}
}

int mergeSort(int * list, int size, int start)
{
	int index, i, j;
	int * listA = list;
	int * tmpA;

	if(isVerbose)
	{
		printf("\nTesting mergeSort 1\n");
	}

	for(index = start; index < size; index = start * 2)
	{
		for(j = 0; j < size; j = j + 2 * index)
		{
			merge(listA, tmpA, j, min(j+index, size), min(j+(index * 2), size));
		}

		int * tmp_ptr = tmpA;
		tmpA = listA;
		listA = tmp_ptr;
	}

	for(i = 0; i < size; i++)
	{
		list[i] = listA[i];
	}

	return 0;
}

int * mergeProcess(int * list, int size)
{
	int n = tasks * size;
	int * listA;

	if(isVerbose)
	{
		printf("\nTesting mergeProcess 1\n");
	}

	if(rank == 0)
	{
		MPI_Gather(list, size, MPI_INT, listA, size, MPI_INT, 0, MPI_COMM_WORLD);
	}

	else
	{
		return NULL;
	}

	mergeSort(listA, n, size);

	return listA;
}

int main(int argc, char *argv[])
{
	int size = LIST_SIZE;
	int retVal, index;
	retVal = MPI_Init(&argc, &argv);
	//vector<int> list(size);
	//vector<int> tmp(size);
	//vector<int> final(size);
	int list[size];

	if(retVal != MPI_SUCCESS)
	{
		perror("MPI Error\n");
		MPI_Abort(MPI_COMM_WORLD, retVal);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &tasks);

	startTime = MPI_Wtime();

	populateList(list, size);
	mergeSort(list, size, 1);
	int * final = mergeProcess(list, size);

	endTime = MPI_Wtime();
	double totalTime = startTime - endTime;

	printf("Total Runtime: %f seconds\n", totalTime);

	if(isVerbose == 1)
	{
		printf("\nSorted List: \n");

		for(index = 0; index < size; index++)
		{
			printf("%d ", final[index]);
		}

		printf("\n");
	}

	MPI_Finalize();
	return(EXIT_SUCCESS);
}