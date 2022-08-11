/* FILE: omp_bug5.cpp
   DESCRIPTION:
      This program fails due to incorrectly scoped variables.  
      Compare to omp_orphan.c.
   AUTHOR: Blaise Barney  6/05
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

// global variables
float a[VECLEN], b[VECLEN];

// external function
float dotprod() {

  // local variables
  int i, tid;
  float sum;

  tid = omp_get_thread_num();
# pragma omp for reduction(+:sum)
  for (i=0; i<VECLEN; i++) {
    sum = sum + (a[i]*b[i]);
    printf("  tid = %i,  i = %i\n", tid, i);
  }
  return sum;
}


// main routine
int main (int argc, char *argv[]) {

  // local variables
  int i;
  float sum;

  // initialize values
  for (i=0; i<VECLEN; i++)   a[i] = b[i] = 1.0 * i;
  sum = 0.0;

  // begin parallel region, with orphaned call to dotprod()
# pragma omp parallel shared(sum)
  sum = dotprod();

  // output result
  printf("Sum = %g\n", sum);

  return 0;
} // end main
