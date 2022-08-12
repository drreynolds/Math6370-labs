/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "mpi.h"


// Example routine using the basic 6 MPI functions
int main(int argc, char* argv[]) {

  // intialize MPI, get total number of processes, this proc's ID
  int ierr, numprocs, myid;
  ierr = MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  // set problem parameters
  int n = 10000000;

  // root node outputs parallelism information to screen
  if (myid == 0)
    std::cout << " starting MPI with " << numprocs << " processes\n";

  // determine this proc's interval in overall problem domain
  // (assumes homogeneous system, giving all procs equal work)
  int is = floor(1.0*n/numprocs)*myid + 1;
  int ie = floor(1.0*n/numprocs)*(myid+1);
  if (myid == numprocs-1)  ie = n;

  // initialize the vectors (only those parts that reside on this proc)
  double *a = new double[ie-is+1];
  double *b = new double[ie-is+1];
  for (int i=0; i<(ie-is+1); i++) {
    a[i] = 0.001 * (i+is) / n;
    b[i] = 0.001 * (n-i-is) / n;
  }

  // start timer
  double stime = MPI_Wtime();

  // perform local portion of dot-product
  double mysum = 0.0;
  for (int i=0; i<(ie-is+1); i++)  mysum += a[i]*b[i];

  // root node collects result
  double sum;
  ierr = MPI_Reduce(&mysum, &sum, 1, MPI_DOUBLE,
		    MPI_SUM, 0, MPI_COMM_WORLD);

  // stop timer
  double ftime = MPI_Wtime();
  double runtime = ftime - stime;

  // root node outputs compute value and runtime
  if (myid == 0) {
    std::cout << " dot-product = " << std::setprecision(12) << sum << std::endl;
    std::cout << "     runtime = " << std::setprecision(12) << runtime << std::endl;
  }

  // free vectors
  delete[] a;
  delete[] b;

  // finalize MPI
  ierr = MPI_Finalize();

} // end main
