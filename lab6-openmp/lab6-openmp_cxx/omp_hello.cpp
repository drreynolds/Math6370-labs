/* FILE: omp_hello.cpp
   DESCRIPTION:
      OpenMP Example - Hello World - C++ Version
      In this simple example, the master thread forks a parallel region.
      All threads in the team obtain their unique thread number and print it.
      The master thread only prints the total number of threads.  Two OpenMP
      library routines are used to obtain the number of threads and each
      thread's number.
   AUTHOR: Blaise Barney  5/99
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

  // local variables
  int nthreads, tid;

  // Fork a team of threads giving them their own copies of variables
# pragma omp parallel private(nthreads, tid)
  {

    // Obtain thread number
    tid = omp_get_thread_num();
    printf("Hello World from thread = %i\n", tid);

    // Only master thread does this
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %i\n", nthreads);
    }

  } // All threads join master thread and disband
  
  return 0;
}  // end main
