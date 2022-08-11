/* FILE: omp_workshare2.cpp
   DESCRIPTION:
      OpenMP Example - Sections Work-sharing - C++ Version
      In this example, the OpenMP SECTION directive is used to assign
      different array operations to each thread that executes a SECTION. 
   AUTHOR: Blaise Barney  5/99
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 50

int main (int argc, char *argv[]) {

  // local variables
  int i, nthreads, tid;
  float a[N], b[N], c[N], d[N];

  // Some initializations
  for (i=0; i<N; i++) {
    a[i] = i * 1.5;
    b[i] = i + 22.35;
    c[i] = d[i] = 0.0;
  }

# pragma omp parallel shared(a,b,c,d,nthreads) private(i,tid)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %i\n", nthreads);
    }
    printf("Thread %i starting...\n", tid);

#   pragma omp sections nowait
    {
#     pragma omp section
      {
	printf("Thread %i doing section 1\n", tid);
	for (i=0; i<N; i++) {
	  c[i] = a[i] + b[i];
	  printf("Thread %i: c[%i] = %g\n", tid, i, c[i]);
        }
      }

#     pragma omp section
      {
	printf("Thread %i doing section 2\n", tid);
	for (i=0; i<N; i++) {
	  d[i] = a[i] * b[i];
	  printf("Thread %i: d[%i] = %g\n", tid, i, d[i]);
        }
      }
    }  // end sections

    printf("Thread %i done.\n", tid);

  }  // end parallel

  return 0;
} // end main
