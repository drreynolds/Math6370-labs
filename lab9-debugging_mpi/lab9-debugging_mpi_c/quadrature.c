/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Inclusions */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

/* macros */
#define PI 3.14159265358979323846

/* Prototypes */
inline double f(double a, double b) { return (a*exp(3.0*a)*sin(25.0*PI*b)); }


/* Main routine to approximate the 2D integral
      int_Omega f(x,y) dx dy
   where Omega is the unit square, [0,1]x[0,1].
   We use a composite Gaussian quadrature rule with 8 points
   per subinterval in each direction (64 points per sub-square),
   over sub-squares of fixed size 1/n x 1/n. */
int main(int argc, char* argv[]) {

  /* declarations */
  int n = 1000;
  int i, j, k, l, is, ie, js, je, ierr, numprocs, myid;
  double F, F_all, h, x, y, a, b, stime, ftime, runtime;
  double F_true = 2.0/225.0/PI*(2.0*exp(3.0)+1.0);
  int nodes = 8;
  double z[] = { -0.18343464249564980493,
		  0.18343464249564980493,
		 -0.52553240991632898581,
		  0.52553240991632898581,
		 -0.79666647741362673959,
 		  0.79666647741362673959,
		 -0.96028985649753623168,
		  0.96028985649753623168 };
  double w[] = {  0.36268378337836198296,
		  0.36268378337836198296,
		  0.31370664587788728733,
		  0.31370664587788728733,
		  0.22238103445337447054,
		  0.22238103445337447054,
		  0.10122853629037625915,
		  0.10122853629037625915 };

  // intialize MPI
  ierr = MPI_Init(&argc, &argv);
  if (ierr != MPI_SUCCESS) {
    fprintf(stderr," error in MPI_Init = %i\n",ierr);
    return 1;
  }
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  if (ierr != MPI_SUCCESS) {
    fprintf(stderr," error in MPI_Comm_size = %i\n",ierr);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  if (ierr != MPI_SUCCESS) {
    fprintf(stderr," error in MPI_Comm_rank = %i\n",ierr);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // root outputs parallelism information to screen
  if (myid == 0)
    printf(" Running with %i MPI tasks\n",numprocs);

  /* start timer */
  stime = MPI_Wtime();

  /* set subinterval width */
  h = 1.0 / n;

  // determine this processor's sub-region of the 2D region
  is = ((int) (1.0*n/numprocs))*myid;
  ie = ((int) (1.0*n/numprocs))*(myid+1);
  if (myid == numprocs-1)  ie = n;

  js = ((int) (1.0*n/numprocs))*myid;
  je = ((int) (1.0*n/numprocs))*(myid+1);
  if (myid == numprocs-1)  je = n;

  /* initialize local result */
  F = 0.0;

  /* perform integration over n intervals in each direction */
  for (i=is; i<ie; i++) {
    for (j=js; j<je; j++) {

      /* in each sub-square, evaluate at all 64 points and combine results */
      for (k=0; k<nodes; k++) {
        for (l=0; l<nodes; l++) {

          /* location of sub-square center */
          x = h * (i + 0.5);
          y = h * (j + 0.5);

          /* location of quadrature evaluation point */
          a = x + 0.5*h*z[k];
          b = y + 0.5*h*z[l];

          /* update numerical integral with contribution from point */
          F += 0.25*h*h*w[k]*w[l]*f(a,b);

        }  /* end l loop */
      }  /* end k loop */
    }  /* end j loop */
  }  /* end i loop */

  // perform reduction to get result on root process
  ierr = MPI_Reduce(&F, &F_all, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (ierr != MPI_SUCCESS) {
    fprintf(stderr," error in MPI_Reduce = %i\n",ierr);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  /* stop timer */
  ftime = MPI_Wtime();
  runtime = ftime - stime;

  /* root outputs computed value and error */
  if (myid == 0) {
    printf(" computed F = %.16e\n",F_all);
    printf("     true F = %.16e\n",F_true);
    printf("      error = %.4e\n",fabs(F_true-F_all));
    printf("    runtime = %g\n",runtime);
  }

  /* finalize MPI */
  ierr = MPI_Finalize();

} /* end main */
