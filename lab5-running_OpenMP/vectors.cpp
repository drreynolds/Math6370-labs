/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include <cmath>


/* Description:
      Computes the linear combination z = a*x + b*y
   Arguments:
      n - int (input), length of vectors
      a - double (input), scalar
      x - double (input), vector
      b - double (input), scalar
      y - double (input), vector
      z - double (output), vector  */
void vector_sum(int n, double a, double *x, double b, double *y, double *z) {

  // loop over the array to compute the vector sum
  int i;
  #pragma omp parallel for shared(n,a,b,x,y,z) private(i)
  for (i=0; i<n; i++)   z[i] = a*x[i] + b*y[i];

} // end vector_sum


/* Description:
      Computes the scaled vector z = a*x
   Arguments:
      n - int (input), length of vectors
      a - double (input), scalar
      x - double (input), vector
      z - double (output), vector  */
void vector_scale(int n, double a, double *x, double *z) {

  // loop over the array to compute the vector sum
  int i;
  #pragma omp parallel for shared(n,a,x,z) private(i)
  for (i=0; i<n; i++)  z[i] = a*x[i];

} // end vector_scale


/* Description:
      Computes the product, z(i) = x(i)*y(i)
   Arguments:
      n - int (input), length of vector
      x - double (input), vector
      y - double (input), vector
      z - double (output), vector  */
void vector_product(int n, double *x, double *y, double *z) {

  // loop over the array to compute the product
  int i;
  #pragma omp parallel for shared(n,x,y,z) private(i)
  for (i=0; i<n; i++)  z[i] = x[i]*y[i];

} // end vector_product


/* Description:
      Computes the power, y(i) = x(i)**e  [requires e >= 0]
   Arguments:
      n - int (input), length of vector
      x - double (input), vector
      e - double (input), scalar
      y - double (output), vector  */
void vector_pow(int n, double *x, double e, double *y) {

  // check that the exponent is non-negative
  if (e < 0) {
    std::cerr << "sorry, vector_pow requires non-negative exponent, e = " << e << std::endl;
    return;
  }

  // loop over the array to compute the power
  int i;
  #pragma omp parallel for shared(n,x,y,e) private(i)
  for (i=0; i<n; i++)  y[i] = pow(x[i],e);

} // end vector_pow


/* Description:
      Computes the vector root-mean-squared norm, sqrt(x \dot x)/n
   Arguments:
      n - int (input), length of vectors
      x - double (input), vector
   Ouptut - double (output), norm of vector  */
double vector_rmsnorm(int n, double *x) {

  // initialize output
  double sum = 0.0;

  // loop over the dimensions to compute the vector sum
  int i;
  #pragma omp parallel for shared(n,x) private(i) reduction(+:sum)
  for (i=0; i<n; i++)  sum += x[i]*x[i]/n;

  return sqrt(sum);

} // end vector_rmsnorm
