/* FILE: omp_mm.cpp
   DESCRIPTION:  
      OpenMp Example - Matrix Multiply - C++ Version
      Demonstrates a matrix multiply using OpenMP. Threads share 
      row iterations according to a predefined chunk size.
   AUTHOR: Blaise Barney
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define NRA 62       // number of rows in matrix A
#define NCA 15       // number of columns in matrix A
#define NCB 7        // number of columns in matrix B


int main (int argc, char *argv[]) {

  // local variables
  int tid, nthreads, i, j, k, chunk;
  double a[NRA][NCA],   // matrix A to be multiplied
         b[NCA][NCB],   // matrix B to be multiplied
         c[NRA][NCB];   // result matrix C
  chunk = 10;           // set loop iteration chunk size

  // Spawn a parallel region explicitly scoping all variables
# pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {

    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Starting matrix multiply example with %i threads\n", 
	     nthreads);
      printf("Initializing matrices...\n");
    }

    // Initialize matrices
#   pragma omp for schedule (static, chunk) 
    for (i=0; i<NRA; i++)
      for (j=0; j<NCA; j++)
	a[i][j]= i+j;

#   pragma omp for schedule (static, chunk)
    for (i=0; i<NCA; i++)
      for (j=0; j<NCB; j++)
	b[i][j]= i*j;

#   pragma omp for schedule (static, chunk)
    for (i=0; i<NRA; i++)
      for (j=0; j<NCB; j++)
	c[i][j]= 0;

    // Do matrix multiply sharing iterations on outer loop 
    // Display who does which iterations for demonstration purposes
    printf("Thread %i starting matrix multiply...\n", tid);
#   pragma omp for schedule (static, chunk)
    for (i=0; i<NRA; i++) {
      printf("Thread = %i did row = %i\n", tid, i);
      for(j=0; j<NCB; j++)
	for (k=0; k<NCA; k++)
	  c[i][j] += a[i][k] * b[k][j];
    }
  }  // end parallel region

  // Print results
  printf("******************************************************\n");
  printf("Result Matrix:\n");
  for (i=0; i<NRA; i++) {
    for (j=0; j<NCB; j++) 
      printf("%g   ", c[i][j]);
    printf("\n"); 
  }
  printf("******************************************************\n");
  printf("Done.\n");

  return 0;
}  // end main
