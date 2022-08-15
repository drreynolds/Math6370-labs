/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include "mpi.h"

// Prototypes
int linresid(double *, double *, double *, double *, double *,
	     double *, double &, int, MPI_Comm);


/* Description: Solves the linear system
             a(k)*u(k-1) + b(k)*u(k) + c(k)*u(k+1) = r(k),
     using a parallelized Jacobi iterative solver. */
int jacobi_solve(double *a, double *b, double *c, double *u,
		 double *r, double *res, int loc_N, double delta,
		 int maxiter, int &iters, MPI_Comm comm) {

  // compute initial linear residual
  double resid2;
  if (linresid(a, b, c, u, r, res, resid2, loc_N, comm) != 0) {
    std::cerr << " jacobi_solve error: linresid failed!\n";
    return 1;
  }

  // iterate until resid2 < delta (or maxiter iterations)
  for (iters=0; iters<=maxiter; iters++) {

    // check for convergence
    if (resid2 < delta)  break;

    // update u = u - diag(b)^{-1}*res
    for (int i=0; i<loc_N; i++)   u[i] = u[i] - res[i]/b[i];

    // compute linear residual
    if ( linresid(a, b, c, u, r, res, resid2, loc_N, comm) != 0) {
      std::cerr << " jacobi_solve error: linresid failed!\n";
      return 1;
    }
  } // for iters

  if (iters == maxiter)
    std::cerr << " jacobi_solve warning: reached maximum iteration limit!\n";

  return 0;

} // end jacobi_solve
