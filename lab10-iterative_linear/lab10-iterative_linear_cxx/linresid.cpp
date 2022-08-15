/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <cmath>


// Description: calculates the linear residual and its averaged 2-norm (WRMS)
int linresid(double *a, double *b, double *c, double *u,
	     double *r, double *res, double &norm2, int N) {

  // compute linear residual at left of subdomain
  res[0] = b[0]*u[0] + c[0]*u[1] - r[0];
  norm2 = res[0]*res[0];

  // compute linear residual in interior of subdomain
  int k;
  for (k=1; k<N-1; k++) {
    res[k] = a[k]*u[k-1] + b[k]*u[k] + c[k]*u[k+1] - r[k];
    norm2 += res[k]*res[k];
  }

  // compute linear residual at right end of subdomain
  k = N-1;
  res[k] = a[k]*u[k-1] + b[k]*u[k] - r[k];
  norm2 += res[k]*res[k];

  // compute averaged 2-norm
  norm2 = sqrt(norm2)/N;

  return 0;
} // end linresid
