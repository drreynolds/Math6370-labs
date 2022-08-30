/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370
*/

#include <cmath>

// Description: computes the 1-norm of the vector u (of length n)
double one_norm(int n, double* u) {

  double sum=0.0;
  for (int i=0; i<n; i++)
    sum += std::abs(u[i]);

  return sum;
}
