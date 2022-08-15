/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "mpi.h"

// Prototypes
int linresid(double *, double *, double *, double *, double *,
	     double *, double &, int, MPI_Comm);
int jacobi_solve(double *, double *, double *, double *, double *,
		 double *, int, double, int, int &, MPI_Comm);


/* We set up and solve the linear system
             (I + gamma*L)u = r,
   where L is a standard 1D Laplace operator, r is a given
   right-hand side, and u is the solution, using a parallelized
   Jacobi iterative solver.

   Requires three input arguments: gamma, delta, and n
   (the size the global domain). */
int main(int argc, char* argv[]) {

  // intialize MPI
  int nprocs, my_id;
  if ( MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    std::cerr << "Error in MPI_Init\n";
    return 1;
  }
  MPI_Comm comm = MPI_COMM_WORLD;
  if ( MPI_Comm_size(comm, &nprocs) != MPI_SUCCESS) {
    std::cerr << "Error in MPI_Comm_size\n";
    MPI_Abort(comm,1);
  }
  if ( MPI_Comm_rank(comm, &my_id) != MPI_SUCCESS) {
    std::cerr << "Error in MPI_Comm_rank\n";
    MPI_Abort(comm,1);
  }

  // root gets problem information from input file
  double gamma, delta;
  int glob_N;
  if (my_id == 0) {
    FILE *FID = fopen("input.txt","r");
    fscanf(FID,"  gamma = %lf,\n",   &gamma);
    fscanf(FID,"  delta = %lf,\n",   &delta);
    fscanf(FID,"  global_N = %i,\n", &glob_N);
    fclose(FID);
  }

  // root node broadcasts information to other procs
  if ( MPI_Bcast(&gamma, 1, MPI_DOUBLE, 0, comm) != MPI_SUCCESS ) {
    std::cerr << " error in MPI_Bcast\n";
    MPI_Abort(comm,1);
  }
  if ( MPI_Bcast(&delta, 1, MPI_DOUBLE, 0, comm) != MPI_SUCCESS) {
    std::cerr << " error in MPI_Bcast\n";
    MPI_Abort(comm,1);
  }
  if ( MPI_Bcast(&glob_N, 1, MPI_INT, 0, comm) != MPI_SUCCESS) {
    std::cerr << " error in MPI_Bcast\n";
    MPI_Abort(comm,1);
  }

  // set local mesh sizes (last process takes up remainder)
  int loc_N = glob_N/nprocs;
  if ((my_id == nprocs-1) && (loc_N*nprocs != glob_N))
    loc_N = glob_N - loc_N*(nprocs-1);

  // root outputs some information to screen
  if (my_id == 0) {
    std::cout << "iterative test with " << nprocs << " processors\n";
    std::cout << "    gamma = " << gamma << "\n";
    std::cout << "    linear solver tolerance delta = " << delta << "\n";
    std::cout << "    global problem size N = " << glob_N << "\n";
    std::cout << "    local problem sizes n = " << loc_N << "\n";
  }

  // Allocate memory
  double *u   = new double[loc_N];
  double *r   = new double[loc_N];
  double *a   = new double[loc_N];
  double *b   = new double[loc_N];
  double *c   = new double[loc_N];
  double *res = new double[loc_N];

  // Set up matrix arrays, right-hand side, and initial solution guess
  for (int k=0; k<loc_N; k++) {
    u[k] =  0.0;
    r[k] =  1.0;
    a[k] = -gamma;
    b[k] =  1.0+gamma*2.0;
    c[k] = -gamma;
  }

  // Adjust a, c arrays if we are at either end of domain
  if (my_id == 0)         a[0] = 0.0;
  if (my_id == nprocs-1)  c[loc_N-1] = 0.0;

  // check linear residual
  double err2norm;
  if ( linresid(a, b, c, u, r, res, err2norm, loc_N, comm) != 0) {
    std::cerr << "iterative test error: linresid failed\n";
    MPI_Abort(comm,1);
  }
  if (my_id == 0)
    std::cout << " initial residual: ||T*u-r||_2 = " << err2norm << "\n";

  // Wait until all procs have caught up
  if ( MPI_Barrier(comm) != 0) {
    std::cerr << "iterative test error: failed MPI_Barrier\n";
    MPI_Abort(comm,1);
  }

  // Solve system, get timing information
  int maxiter = 10000;
  double stime = MPI_Wtime();
  int iters;
  if ( jacobi_solve(a, b, c, u, r, res, loc_N,
		    delta, maxiter, iters, comm) != 0) {
    std::cerr << "iterative test error: jacobi_solve failed\n";
    MPI_Abort(comm,1);
  }
  double ftime = MPI_Wtime();
  if (my_id == 0) {
    std::cout << " converged in " << iters << " iterations at delta = " << delta << "\n";
    std::cout << " solution time: " << ftime-stime << " seconds\n";
  }

  // check linear residual
  if ( linresid(a, b, c, u, r, res, err2norm, loc_N, comm) != 0) {
    std::cerr << "iterative test error: linresid failed\n";
    MPI_Abort(comm,1);
  }
  if (my_id == 0)
    std::cout << " final residual: ||T*u-r||_2 = " << err2norm << "\n";

  // Free matrix/solver memory
  delete[] u;
  delete[] r;
  delete[] a;
  delete[] b;
  delete[] c;
  delete[] res;

  // finalize MPI
  MPI_Finalize();

  return 0;
} // end main
