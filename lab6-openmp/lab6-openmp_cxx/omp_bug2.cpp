/* FILE: omp_bug2.cpp
   DESCRIPTION:
      Another OpenMP program with a bug. 
   AUTHOR: Blaise Barney 
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

  // local variables
  int nthreads, i, tid;
  float total;

  // Spawn parallel region 
# pragma omp parallel 
  {
    // Obtain thread number
    tid = omp_get_thread_num();
    
    // Only master thread does this
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %i\n", nthreads);
    }

    // Everyone does this
    printf("Thread %i is starting...\n", tid);

#   pragma omp barrier

    // do some work
    total = 0.0;
#   pragma omp for schedule(dynamic,10)
    for (i=0; i<1000000; i++)  total = total + i*1.0;

    printf("Thread %i is done! Total = %g\n", tid, total);

  } // End of parallel region

  return 0;
}  // end main

