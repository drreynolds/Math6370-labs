/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "mpi.h"

// Prototypes
inline double f(double a, double b) { return (a*exp(3.0*a)*sin(25.0*M_PI*b)); }


/* Main routine to approximate the 2D integral
      int_Omega f(x,y) dx dy
   where Omega is the unit square, [0,1]x[0,1].
   We use a composite Gaussian quadrature rule with 8 points
   per subinterval in each direction (64 points per sub-square),
   over sub-squares of fixed size 1/n x 1/n. */
int main(int argc, char* argv[]) {

  // parameter declarations
  const int n=1000;
  const double F_true = 2.0/225.0/M_PI*(2.0*exp(3.0)+1.0);
  const int nodes = 8;
  const double z[] = { -0.18343464249564980493,
                        0.18343464249564980493,
                       -0.52553240991632898581,
                        0.52553240991632898581,
                       -0.79666647741362673959,
                        0.79666647741362673959,
                       -0.96028985649753623168,
                        0.96028985649753623168 };
  const double w[] = {  0.36268378337836198296,
                        0.36268378337836198296,
                        0.31370664587788728733,
                        0.31370664587788728733,
                        0.22238103445337447054,
                        0.22238103445337447054,
                        0.10122853629037625915,
                        0.10122853629037625915 };

  // intialize MPI
  int numprocs, myid;
  int ierr = MPI_Init(&argc, &argv);
  if (ierr != MPI_SUCCESS) {
    std::cerr << " error in MPI_Init = " << ierr << "\n";
    return 1;
  }
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  if (ierr != MPI_SUCCESS) {
    std::cerr << " error in MPI_Comm_size = " << ierr << "\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  if (ierr != MPI_SUCCESS) {
    std::cerr << " error in MPI_Comm_rank = " << ierr << "\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // root outputs parallelism information to screen
  if (myid == 0)
    std::cout << " Running with " << numprocs << " MPI tasks\n";

  // start timer
  double stime = MPI_Wtime();

  // set subinterval width
  double h = 1.0 / n;

  // determine this processor's sub-region of the 2D region
  int is = ((int) (1.0*n/numprocs))*myid;
  int ie = ((int) (1.0*n/numprocs))*(myid+1);
  if (myid == numprocs-1)  ie = n;

  int js = ((int) (1.0*n/numprocs))*myid;
  int je = ((int) (1.0*n/numprocs))*(myid+1);
  if (myid == numprocs-1)  je = n;

  // initialize local result
  double F = 0.0;

  // perform integration over n intervals in each direction
  for (int i=is; i<ie; i++) {
    for (int j=js; j<je; j++) {

      // in each sub-square, evaluate at all 64 points and combine results
      for (int k=0; k<nodes; k++) {
        for (int l=0; l<nodes; l++) {

          // location of sub-square center
          double x = h * (i + 0.5);
          double y = h * (j + 0.5);

          // location of quadrature evaluation point
          double a = x + 0.5*h*z[k];
          double b = y + 0.5*h*z[l];

          // update numerical integral with contribution from point
          F += 0.25*h*h*w[k]*w[l]*f(a,b);

        }  // end l loop
      }  // end k loop
    }  // end j loop
  }  // end i loop

  // perform reduction to get result on root process
  double F_all = 0.0;
  ierr = MPI_Reduce(&F, &F_all, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (ierr != MPI_SUCCESS) {
    std::cerr << " error in MPI_Reduce = " << ierr << "\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // stop timer
  double ftime = MPI_Wtime();
  double runtime = ftime - stime;

  // root outputs computed value and error
  if (myid == 0) {
    std::cout << " computed F = " << std::setprecision(16) << F_all << "\n";
    std::cout << "     true F = " << F_true << "\n";
    std::cout << "      error = " << std::setprecision(5) << std::abs(F_true-F_all) << "\n";
    std::cout << "    runtime = " << runtime << "\n";
  }

  // finalize MPI
  ierr = MPI_Finalize();

} // end main
