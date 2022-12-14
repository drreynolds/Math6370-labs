/* FILE: omp_bug4fix.cpp
   DESCRIPTION:
      The problem in omp_bug4.cpp is that the first thread acquires 
      locka and then tries to get lockb before releasing locka. 
      Meanwhile, the second thread has acquired lockb and then 
      tries to get locka before releasing lockb.  This solution 
      overcomes the deadlock by using locks correctly.
   AUTHOR: Blaise Barney  01/29/04
   UPDATED: Daniel R. Reynolds (updated to C++), 1/13/2013 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N      1000000
#define PI     3.1415926535
#define DELTA  0.01415926535

int main (int argc, char *argv[]) {

  // local variables
  int nthreads, tid, i;
  float a[N], b[N];
  omp_lock_t locka, lockb;

  // Initialize the locks
  omp_init_lock(&locka);
  omp_init_lock(&lockb);

  // Fork a team of threads giving them their own copies of variables
# pragma omp parallel shared(a, b, nthreads, locka, lockb) private(tid)
  {

    // Obtain thread number and number of threads
    tid = omp_get_thread_num();
#   pragma omp master
    {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %i\n", nthreads);
    }
    printf("Thread %i starting...\n", tid);

#   pragma omp barrier

#   pragma omp sections nowait
    {
#     pragma omp section
      {
	printf("Thread %i initializing a[]\n", tid);
	omp_set_lock(&locka);
	for (i=0; i<N; i++)   a[i] = i * DELTA;
	omp_unset_lock(&locka);

	omp_set_lock(&lockb);
	printf("Thread %i adding a[] to b[]\n", tid);
	for (i=0; i<N; i++)   b[i] += a[i];
	omp_unset_lock(&lockb);
      }

#     pragma omp section
      {
	printf("Thread %i initializing b[]\n", tid);
	omp_set_lock(&lockb);
	for (i=0; i<N; i++)   b[i] = i * PI;
	omp_unset_lock(&lockb);
	
	omp_set_lock(&locka);
	printf("Thread %i adding b[] to a[]\n", tid);
	for (i=0; i<N; i++)   a[i] += b[i];
	omp_unset_lock(&locka);
      }
    }  // end sections
  }  // end parallel

  return 0;
}  // end main
