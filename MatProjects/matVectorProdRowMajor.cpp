#include <omp.h>
#include <stdio.h>
#include <iostream>
using namespace std;
const int n = 10000;
const int m = 50000;
int numThreads = 1;
int i,j;
double ** a, ** b, ** c;
double startTime = 0.0, totalTime = 0.0;
int chunk_size = 1, tid;
int main() {

           a = new double*[n];
           a[0] = new double[n*m];
           b = new double*[m];
           b[0] = new double[m*1];
           c = new double*[n];
           c[0] = new double[n*1];


           for (i = 1; i < n; i++){
               a[i] = a[0] + i*m; }
           for (i = 1; i <m; i++){
               b[i] = b[0] + i*1; }
           for (i = 1; i < n; i++){
               c[i] = c[0] + i*1; }

           #pragma omp parallel num_threads(numThreads)
               {numThreads = omp_get_num_threads();}
               chunk_size = n/numThreads;
               printf(" number of threads = %d \n", numThreads);
           startTime = omp_get_wtime();
           #pragma omp parallel for  num_threads(numThreads) schedule(static,chunk_size)
                    for (j = 0; j < m; j++)
                    {
                       for (i = 0; i < n; i++){
                           c[i][1] = c[i][1] + a[i][j]*  b[j][1];
                        }

                       }// end of parallel region                                                                      

                     totalTime = omp_get_wtime() - startTime;
                     tid = omp_get_thread_num();
                     printf(" myId = %d \n", tid);
                     printf("Thread %d spent = %f sec\n",tid, totalTime);

                     delete[] a, b, c;

                     return 0;
}