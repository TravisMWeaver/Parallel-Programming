#include <omp.h>
#include <iostream>
using namespace std;
const int n = 1000;
const int m = 8000;
const int r = 900;
int numThreads = 2;
double temp = 0.0, currentTime = 0.0, computationTime = 0.0; 
int i,j,k;
double ** a, ** b,** c, ** bt;
int tid;
int main() {
   //initialize the matrices
   a = new double*[n];
   a[0] = new double[n*m];
   b = new double*[m];
   b[0] = new double[m*r];
   c = new double*[n];
   c[0] = new double[n*r];
   bt = new double*[r];
   bt[0] = new double[r*m];


   for (i = 1; i < n; i++){
       a[i] = a[0] + i*m; }
   for (i = 1; i <m; i++){
       b[i] = b[0] + i*r; }
   for (i = 1; i < n; i++){
       c[i] = c[0] + i*r; }
   for ( i = 1; i <r; i++){
       bt[i] = bt[0]+i*m;}
//*
      
   for (i = 0; i < n; i++){
      for (j = 0; j < m; j++) {
         a[i][j] = 0.01 +i + (i + 1)*(j+1);
      }
   }
   for (i = 0; i < m; i++){
      for (j = 0; j < r; j++) {
         b[i][j] = 0.01+ i + (i + 1)*(j+1);
      }
   }
   for (i = 0; i < n; i++){
      for (j = 0; j < r; j++) {
         c[i][j] = 0.01 +i + (i + 1)*(j+1);
      }
   }
  // transpose matrix b
   for ( i = 0; i < m; i++){
       for ( j = 0; j < r; j++){
            bt[j][i]= b[i][j];
       }
   }
   // multiply matrices a and b to obtain matrix c 
   //#pragma omp parellel for
   #pragma omp parallel num_threads(numThreads)
        {numThreads = omp_get_num_threads();}
        printf(" number of threads = %d \n", numThreads);
   currentTime = omp_get_wtime();

   #pragma omp parallel for num_threads(numThreads) schedule(static)
   for (i = 0; i < n; i++) {
      for (j = 0; j < r; j++) {
         temp = 0.0;
         for ( k = 0; k < m; k++){ 
         //cout << "from tid " << tid <<" ";
         temp = c[i][j] + a[i][k]*bt[j][k];
         //cout << "c[" << i <<","<< j << "] = ";
         //cout.width(4);
         //cout << left<<c[i][j];
      }
      c[i][j] = temp;
     //cout << "\n";
   }
 }
  computationTime = omp_get_wtime() - currentTime; 
  cout << "Time elapsed = " << computationTime  << "\n"; //*/
  delete[] a, b,c,bt; 
return 0;
}
