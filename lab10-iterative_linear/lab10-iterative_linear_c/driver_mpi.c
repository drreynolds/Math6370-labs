/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Inclusions */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

/* Prototypes */
int linresid(double *, double *, double *, double *, double *,
	     double *, double *, int, MPI_Comm);
int jacobi_solve(double *, double *, double *, double *, double *,
		 double *, int, double, int, int *, MPI_Comm);


/* We set up and solve the linear system
             (I + gamma*L)u = r,
   where L is a standard 1D Laplace operator, r is a given
   right-hand side, and u is the solution, using a parallelized
   Jacobi iterative solver.

   Requires three input arguments: gamma, delta, and n
   (the size the global domain). */
int main(int argc, char* argv[]) {

  /* local variables */
  int nprocs, my_id, glob_N, loc_N, k, maxiter, iters;
  double gamma, delta, err2norm, stime, ftime;
  double *u, *r, *a, *b, *c, *res;
  MPI_Comm comm;
  FILE* FID;

  /* intialize MPI */
  if ( MPI_Init(&argc, &argv) != MPI_SUCCESS) {
    fprintf(stderr,"Error in MPI_Init\n");
    return 1;
  }
  comm = MPI_COMM_WORLD;
  if ( MPI_Comm_size(comm, &nprocs) != MPI_SUCCESS) {
    fprintf(stderr,"Error in MPI_Comm_size\n");
    MPI_Abort(comm,1);
  }
  if ( MPI_Comm_rank(comm, &my_id) != MPI_SUCCESS) {
    fprintf(stderr,"Error in MPI_Comm_rank\n");
    MPI_Abort(comm,1);
  }

  /* root node gets problem information from input file */
  if (my_id == 0) {
    FID = fopen("input.txt","r");
    fscanf(FID,"  gamma = %lf,\n",   &gamma);
    fscanf(FID,"  delta = %lf,\n",   &delta);
    fscanf(FID,"  global_N = %i,\n", &glob_N);
    fclose(FID);
  }

  /* root node broadcasts information to other procs */
  if ( MPI_Bcast(&gamma, 1, MPI_DOUBLE, 0, comm) != MPI_SUCCESS ) {
    fprintf(stderr," error in MPI_Bcast\n");
    MPI_Abort(comm,1);
  }
  if ( MPI_Bcast(&delta, 1, MPI_DOUBLE, 0, comm) != MPI_SUCCESS) {
    fprintf(stderr," error in MPI_Bcast\n");
    MPI_Abort(comm,1);
  }
  if ( MPI_Bcast(&glob_N, 1, MPI_INT, 0, comm) != MPI_SUCCESS) {
    fprintf(stderr," error in MPI_Bcast\n");
    MPI_Abort(comm,1);
  }

  /* set local mesh sizes (last process takes up remainder) */
  loc_N = glob_N/nprocs;
  if ((my_id == nprocs-1) && (loc_N*nprocs != glob_N))
    loc_N = glob_N - loc_N*(nprocs-1);

  /* root node outputs some information to screen */
  if (my_id == 0) {
    printf("iterative test with %i processors\n",nprocs);
    printf("    gamma = %g\n",gamma);
    printf("    linear solver tolerance delta = %g\n",delta);
    printf("    global problem size N = %i\n",glob_N);
    printf("    local problem sizes n = %i\n",loc_N);
  }

  /* Allocate memory */
  u = (double *) malloc(loc_N * sizeof(double));
  r = (double *) malloc(loc_N * sizeof(double));
  a = (double *) malloc(loc_N * sizeof(double));
  b = (double *) malloc(loc_N * sizeof(double));
  c = (double *) malloc(loc_N * sizeof(double));
  res = (double *) malloc(loc_N * sizeof(double));

  /* Set up matrix arrays, right-hand side, and initial solution guess */
  for (k=0; k<loc_N; k++) {
    u[k] =  0.0;
    r[k] =  1.0;
    a[k] = -gamma;
    b[k] =  1.0+gamma*2.0;
    c[k] = -gamma;
  }

  /* Adjust a, c arrays if we are at either end of domain */
  if (my_id == 0)         a[0] = 0.0;
  if (my_id == nprocs-1)  c[loc_N-1] = 0.0;

  /* check linear residual */
  if ( linresid(a, b, c, u, r, res, &err2norm, loc_N, comm) != 0) {
    fprintf(stderr,"iterative test error: linresid failed\n");
    MPI_Abort(comm,1);
  }
  if (my_id == 0)
    printf(" initial residual: ||T*u-r||_2 = %g\n", err2norm);

  /* Wait until all procs have caught up */
  if ( MPI_Barrier(comm) != 0) {
    fprintf(stderr,"iterative test error: failed MPI_Barrier\n");
    MPI_Abort(comm,1);
  }

  /* Solve system, get timing information */
  maxiter = 10000;
  stime = MPI_Wtime();
  if ( jacobi_solve(a, b, c, u, r, res, loc_N,
		    delta, maxiter, &iters, comm) != 0) {
    fprintf(stderr,"iterative test error: jacobi_solve failed\n");
    MPI_Abort(comm,1);
  }
  ftime = MPI_Wtime();
  if (my_id == 0) {
    printf(" converged in %i iterations at delta = %g\n", iters, delta);
    printf(" solution time: %g seconds\n", ftime-stime);
  }

  /* check linear residual */
  if ( linresid(a, b, c, u, r, res, &err2norm, loc_N, comm) != 0) {
    fprintf(stderr,"iterative test error: linresid failed\n");
    MPI_Abort(comm,1);
  }
  if (my_id == 0)
    printf(" final residual: ||T*u-r||_2 = %g\n", err2norm);

  /* Free matrix/solver memory */
  free(u);
  free(r);
  free(a);
  free(b);
  free(c);
  free(res);

  /* finalize MPI */
  MPI_Finalize();

  return 0;
} /* end main */
