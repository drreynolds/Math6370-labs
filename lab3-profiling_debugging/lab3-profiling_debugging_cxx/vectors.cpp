/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <cmath>


/* Description:
      Computes the linear combination z = x + y
   Arguments:
      l - int (input), length of vectors
      m - int (input), length of vectors
      n - int (input), length of vectors
      x - double (input), vector
      y - double (input), vector
      z - double (output), vector */
void vector_sum(int l, int m, int n, double ***x, double ***y, double ***z) {

  // loop over the dimensions to compute the vector sum
  for (int k=0; k<n; k++)
    for (int j=0; j<m; j++)
      for (int i=0; i<l; i++)
	z[i][j][k] = x[i][j][k] + y[i][j][k];

} // end vector_sum


/* Description:
      Computes the scaled vector z = a*x
   Arguments:
      l - int (input), length of vectors
      m - int (input), length of vectors
      n - int (input), length of vectors
      a - double (input), scalar
      x - double (input), vector
      z - double (output), vector */
void vector_scale(int l, int m, int n, double a, double ***x, double ***z) {

  // loop over the dimensions to compute the vector sum
  for (int i=0; i<l; i++)
    for (int j=0; j<m; j++)
      for (int k=0; k<n; k++)
	z[i][j][k] = a*x[i][j][k];

} // end vector_scale


/* Description:
      Computes the infinity-norm, norm = ||x||_{\infty}
   Arguments:
      l - int (input), length of vector
      m - int (input), length of vector
      n - int (input), length of vector
      x - double (input), vector
      Output - double, norm of vector */
double vector_infnorm(int l, int m, int n, double ***x) {

  // loop over the dimensions to compute the vector sum
  double norm=0.0;
  for (int k=0; k<n; k++)
    for (int j=0; j<m; j++)
      for (int i=0; i<l; i++)
	norm = (std::abs(x[i][j][k]) > norm) ? std::abs(x[i][j][k]) : norm;
  return norm;

} // end vector_infnorm


/* Description:
      Computes the dot-product, prod = x \dot y
   Arguments:
      l - int (input), length of vectors
      m - int (input), length of vectors
      n - int (input), length of vectors
      x - double (input), vector
      y - double (input), vector
      Output - double, dot-product of vectors
*/
double vector_dotprod(int l, int m, int n, double ***x, double ***y) {

  // loop over the dimensions to compute the vector sum
  double prod=0.0;
  for (int k=0; k<n; k++)
    for (int j=0; j<m; j++)
      for (int i=0; i<l; i++)
	prod += x[i][j][k] * y[i][j][k];
  return prod;

} // end vector_dotprod
