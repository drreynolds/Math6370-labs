/* FILE: omp_orphan.cpp
   DESCRIPTION:
      OpenMP Example - Parallel region with an orphaned directive - C++ Version 
      This example demonstrates a dot product being performed by an orphaned
      loop reduction construct.  Scoping of the reduction variable is critical.
   AUTHOR: Blaise Barney  5/99
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

// global variables
float a[VECLEN], b[VECLEN], sum;

// external function
void dotprod() {

  // local variables 
  int i, tid;

  tid = omp_get_thread_num();
# pragma omp for reduction(+:sum)
  for (i=0; i < VECLEN; i++) {
    sum = sum + (a[i]*b[i]);
    printf("  tid = %i,  i = %i\n", tid, i);
  }
}


// main routine
int main (int argc, char *argv[]) {

  // local variables
  int i;

  // initialize values
  for (i=0; i<VECLEN; i++)   a[i] = b[i] = 1.0 * i;
  sum = 0.0;

  // begin parallel region, with orphaned call to dotprod()
# pragma omp parallel
  dotprod();

  // output result
  printf("Sum = %g\n", sum);

  return 0;
} // end main
