/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <omp.h>

// Prototypes
void vector_sum(int, double, double*, double, double*, double*);
void vector_scale(int, double, double*, double*);
void vector_product(int, double*, double*, double*);
void vector_pow(int, double*, double, double*);
double vector_rmsnorm(int, double*);


/* Allocates and fills the vectors x, y, z.
   Calls a set of vector routines.
   Writes the total time taken for all vector routines. */
int main(int argc, char* argv[]) {

  // set problem parameters
  const int n = 10000000;

  // allocate data arrays
  double *x = new double[n];
  double *y = new double[n];
  double *z = new double[n];

  // fill in vectors x and y, scalars a and b
  for (int i=0; i<n; i++) {
    x[i] = 1.0*(i+1)/n;
    y[i] = 1.0*(n-i-1)/n;
  }
  double a = -2.0;
  double b = 0.2;

  // start timer -- Note: we use the OpenMP timer omp_get_wtime()
  // instead of clock(), since omp_get_wtime() is independent of the
  // number of threads, but clock() adds the time on each thread together
  double stime = omp_get_wtime();

  // call the vector routines a number of times
  for (int i=0; i<20; i++) {
    vector_sum(n, b, x, a, y, z);
    vector_scale(n, b, z, y);
    vector_product(n, y, z, x);
    vector_pow(n, x, b, y);
    a = vector_rmsnorm(n, y);
  }

  // stop timer
  double ftime = omp_get_wtime();
  double runtime = ftime - stime;

  // output result and solution time
  std::cout << " Test run using " << omp_get_max_threads() << " OpenMP threads" << std::endl;
  std::cout << std::setprecision(16) << " Final rms norm = " << a << std::endl;
  std::cout << std::setprecision(10) << " Total run time = " << runtime << std::endl;

  // free data arrays
  delete[] x;
  delete[] y;
  delete[] z;

} // end main
