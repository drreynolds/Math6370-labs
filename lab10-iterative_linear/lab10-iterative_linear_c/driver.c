/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Inclusions */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/* Prototypes */
int linresid(double *, double *, double *, double *, double *,
	     double *, double *, int);
int jacobi_solve(double *, double *, double *, double *, double *,
		 double *, int, double, int, int *);


/* We set up and solve the linear system
           (I + gamma*L)u = r,
   where L is a standard 1D Laplace operator, r is a given
   right-hand side, and u is the solution, using a
   Jacobi iterative solver.

   Requires three input arguments: gamma, delta, and n
   (the size the global domain). */
int main(int argc, char* argv[]) {

  /* local variables */
  int glob_N, k, maxiter, iters;
  double gamma, delta, err2norm;
  double *u, *r, *a, *b, *c, *res;
  FILE* FID;
  clock_t stime, ftime;

  /* get problem information from input file */
  FID = fopen("input.txt","r");
  fscanf(FID,"  gamma = %lf,\n",   &gamma);
  fscanf(FID,"  delta = %lf,\n",   &delta);
  fscanf(FID,"  global_N = %i,\n", &glob_N);
  fclose(FID);

  /* output some information to screen */
  printf("iterative test\n");
  printf("    gamma = %g\n",gamma);
  printf("    linear solver tolerance delta = %g\n",delta);
  printf("    problem size N = %i\n",glob_N);

  /* Allocate memory */
  u   = (double *) malloc(glob_N * sizeof(double));
  r   = (double *) malloc(glob_N * sizeof(double));
  a   = (double *) malloc(glob_N * sizeof(double));
  b   = (double *) malloc(glob_N * sizeof(double));
  c   = (double *) malloc(glob_N * sizeof(double));
  res = (double *) malloc(glob_N * sizeof(double));

  /* Set up matrix arrays, right-hand side, and initial solution guess */
  for (k=0; k<glob_N; k++) {
    u[k] =  0.0;
    r[k] =  1.0;
    a[k] = -gamma;
    b[k] =  1.0+gamma*2.0;
    c[k] = -gamma;
  }

  /* Adjust a, c arrays at ends of domain */
  a[0] = 0.0;
  c[glob_N-1] = 0.0;

  /* check linear residual */
  if ( linresid(a, b, c, u, r, res, &err2norm, glob_N) != 0) {
    fprintf(stderr,"iterative test error: linresid failed\n");
    return 1;
  }
  printf(" initial residual: ||T*u-r||_2 = %g\n", err2norm);

  /* Solve system, get timing information */
  maxiter = 10000;
  stime = clock();
  if ( jacobi_solve(a, b, c, u, r, res, glob_N,
		    delta, maxiter, &iters) != 0) {
    fprintf(stderr,"iterative test error: jacobi_solve failed\n");
    return 1;
  }
  ftime = clock();
  printf(" converged in %i iterations at delta = %g\n", iters, delta);
  printf(" solution time: %g seconds",
	 ((double) (ftime - stime))/CLOCKS_PER_SEC );

  /* check linear residual */
  if ( linresid(a, b, c, u, r, res, &err2norm, glob_N) != 0) {
    fprintf(stderr,"iterative test error: linresid failed\n");
    return 1;
  }
  printf(" final residual: ||T*u-r||_2 = %g\n", err2norm);

  /* Free matrix/solver memory */
  free(u);
  free(r);
  free(a);
  free(b);
  free(c);
  free(res);

  return 0;
} /* end main */
