/* FILE: omp_bug1.cpp
   DESCRIPTION:
   This example attempts to show use of the parallel for construct.  
   However it will generate errors at compile time.  Try to determine 
   what is causing the error.  See omp_bug1fix.cpp for a corrected 
   version.
   AUTHOR: Blaise Barney  5/99
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N           50
#define CHUNKSIZE   5

int main (int argc, char *argv[]) {

  // local variables
  int i, chunk, tid;
  float a[N], b[N], c[N];

  // Some initializations
  for (i=0; i < N; i++)  a[i] = b[i] = i * 1.0;
  chunk = CHUNKSIZE;

# pragma omp parallel for			\
  shared(a,b,c,chunk)				\
  private(i,tid)				\
  schedule(static,chunk)
  {
    tid = omp_get_thread_num();
    for (i=0; i<N; i++) {
      c[i] = a[i] + b[i];
      printf("tid = %i, i = %i, c[i]= %g\n", tid, i, c[i]);
    }
  }  // end parallel for

  return 0;
} // end main

