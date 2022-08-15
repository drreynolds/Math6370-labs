/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

// Prototypes
int linresid(double *, double *, double *, double *, double *,
	     double *, double &, int);
int jacobi_solve(double *, double *, double *, double *, double *,
		 double *, int, double, int, int &);


/* We set up and solve the linear system
           (I + gamma*L)u = r,
   where L is a standard 1D Laplace operator, r is a given
   right-hand side, and u is the solution, using a
   Jacobi iterative solver.

   Requires three input arguments: gamma, delta, and n
   (the size the global domain). */
int main(int argc, char* argv[]) {

  // get problem information from input file
  int glob_N;
  double gamma, delta;
  FILE *FID = fopen("input.txt","r");
  fscanf(FID,"  gamma = %lf,\n",   &gamma);
  fscanf(FID,"  delta = %lf,\n",   &delta);
  fscanf(FID,"  global_N = %i,\n", &glob_N);
  fclose(FID);

  // output some information to screen
  std::cout << "iterative test\n";
  std::cout << "    gamma = " << gamma << "\n";
  std::cout << "    linear solver tolerance delta = " << delta << "\n";
  std::cout << "    problem size N = " << glob_N << "\n";

  // Allocate memory
  double *u   = new double[glob_N];
  double *r   = new double[glob_N];
  double *a   = new double[glob_N];
  double *b   = new double[glob_N];
  double *c   = new double[glob_N];
  double *res = new double[glob_N];

  // Set up matrix arrays, right-hand side, and initial solution guess
  for (int k=0; k<glob_N; k++) {
    u[k] =  0.0;
    r[k] =  1.0;
    a[k] = -gamma;
    b[k] =  1.0+gamma*2.0;
    c[k] = -gamma;
  }

  // Adjust a, c arrays at ends of domain
  a[0] = 0.0;
  c[glob_N-1] = 0.0;

  // check linear residual
  double err2norm;
  if ( linresid(a, b, c, u, r, res, err2norm, glob_N) != 0) {
    std::cerr << "iterative test error: linresid failed\n";
    return 1;
  }
  std::cout << " initial residual: ||T*u-r||_2 = " << err2norm << "\n";

  // Solve system, get timing information
  int maxiter = 10000;
  clock_t stime = clock();
  int iters;
  if ( jacobi_solve(a, b, c, u, r, res, glob_N,
		    delta, maxiter, iters) != 0) {
    std::cerr << "iterative test error: jacobi_solve failed\n";
    return 1;
  }
  clock_t ftime = clock();
  std::cout << " converged in " << iters << " iterations at delta = " << delta << "\n";
  std::cout << " solution time: " << ((double) (ftime - stime))/CLOCKS_PER_SEC << " seconds\n";

  // check linear residual
  if ( linresid(a, b, c, u, r, res, err2norm, glob_N) != 0) {
    std::cerr << "iterative test error: linresid failed\n";
    return 1;
  }
  std::cout << " final residual: ||T*u-r||_2 = " << err2norm << "\n";

  // Free matrix/solver memory
  delete[] u;
  delete[] r;
  delete[] a;
  delete[] b;
  delete[] c;
  delete[] res;

  return 0;
} // end main
