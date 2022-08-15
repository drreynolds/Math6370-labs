/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Inclusions */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Prototypes */
int linresid(double *, double *, double *, double *,
	     double *, double *, double *, int);


/* Description: Solves the linear system
             a(k)*u(k-1) + b(k)*u(k) + c(k)*u(k+1) = r(k),
     using a Jacobi iterative solver. */
int jacobi_solve(double *a, double *b, double *c, double *u, double *r,
		 double *res, int N, double delta, int maxiter, int *iters) {

  /* local variables */
  double resid2;
  int i, its;

  /* compute initial linear residual */
  if ( linresid(a, b, c, u, r, res, &resid2, N) != 0 ) {
    fprintf(stderr," jacobi_solve error: linresid failed!\n");
    return 1;
  }

  /* iterate until resid2 < delta (or maxiter iterations) */
  for (its=0; its<=maxiter; its++) {

    /* check for convergence */
    if (resid2 < delta)  break;

    /* update u = u - diag(b)^{-1}*res */
    for (i=0; i<N; i++)   u[i] = u[i] - res[i]/b[i];

    /* compute linear residual */
    if ( linresid(a, b, c, u, r, res, &resid2, N) != 0) {
      fprintf(stderr," jacobi_solve error: linresid failed!\n");
      return 1;
    }
  } /* for its */

  if (its == maxiter)
    fprintf(stderr,"  jacobi_solve warning: reached maximum iteration limit!\n");

  *iters = its;
  return 0;

} /* end jacobi_solve */
