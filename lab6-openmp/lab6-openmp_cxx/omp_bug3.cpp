/* FILE: omp_bug3.cpp
   DESCRIPTION:
      This very simple program causes a segmentation fault.
   AUTHOR: Blaise Barney  01/09/04
   UPDATED: Daniel R. Reynolds (Updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 1048

int main (int argc, char *argv[]) {

  // local variables
  int nthreads, tid, i, j;
  double a[N][N];

  // Fork a team of threads with explicit variable scoping
# pragma omp parallel shared(nthreads) private(i,j,tid,a)
  {

    // Obtain/print thread info
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %i\n", nthreads);
    }
    printf("Thread %i starting...\n", tid);

    // Each thread works on its own private copy of the array
    for (i=0; i<N; i++)
      for (j=0; j<N; j++)
	a[i][j] = tid + i + j;

    // For confirmation
    printf("Thread %i done. Last element = %g\n", tid, a[N-1][N-1]);

  }  // All threads join master thread and disband

  return 0;
}  // end main

