#include <omp.h>
#include <stdio.h>
#include <iostream>
using namespace std;
const int n = 10000;
const int m = 10000;
int numThreads = 2;
int i,j, a[n][m]= {1}, b[m][1]={2},c[n][1] = {0};
double startTime = 0.0, totalTime = 0.0;
int chunk_size = 1, tid;
int main() {
           #pragma omp parallel num_threads(numThreads)
               {numThreads = omp_get_num_threads();}
               chunk_size = n/numThreads;
               printf(" number of threads = %d \n", numThreads);
           startTime = omp_get_wtime();
           #pragma omp parallel for  num_threads(numThreads) schedule(static,chunk_size)
                    for (i = 0; i < n; i++)
                    {
                       for (j = 0; j< m; j++){
                           c[i][1] = c[i][1] + a[i][j]*  b[j][1];
                        }

                       }// end of parallel region                                                                      

                     totalTime = omp_get_wtime() - startTime;
                     tid = omp_get_thread_num();
                     printf(" myId = %d \n", tid);
                     printf("Thread %d spent = %f sec\n",tid, totalTime);
     return 0;
}