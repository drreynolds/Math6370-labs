/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "mpi.h"

// Prototypes
void chem_solver(double, double*, double*, double*,
		 double, double, int, int*, double*);


/* Example routine to compute the equilibrium chemical densities at
   a number of spatial locations, given a (random) background temperature
   field.  The chemical rate equations and solution strategy are in the
   subroutine chem_solver, which is called at every spatial location. */
int main(int argc, char* argv[]) {

  // initialize MPI
  int ierr = MPI_Init(&argc, &argv);
  if (ierr != MPI_SUCCESS) {
     std::cerr << "Error in calling MPI_Init\n";
     return 1;
  }

  // 1. set solver input parameters
  const int maxit = 1000000;
  const double lam = 1.e-2;
  const double eps = 1.e-10;

  // 2. input the number of intervals
  int n;
  std::cout << "Enter the number of intervals (0 quits):\n";
  std::cin >> n;
  if (n < 1) {
    return 1;
  }

  // 3. allocate temperature and solution arrays
  double *T = new double[n];
  double *u = new double[n];
  double *v = new double[n];
  double *w = new double[n];

  // 4. set random temperature field, initial guesses at chemical densities
  for (int i=0; i<n; i++)  T[i] = random() / (pow(2.0,31.0) - 1.0);
  for (int i=0; i<n; i++)  u[i] = 0.35;
  for (int i=0; i<n; i++)  v[i] = 0.1;
  for (int i=0; i<n; i++)  w[i] = 0.5;

  // 5. start timer
  double stime = MPI_Wtime();

  // 6. call solver over n intervals
  for (int i=0; i<n; i++) {
    int its;
    double res;
    chem_solver(T[i], &(u[i]), &(v[i]), &(w[i]), lam, eps, maxit, &its, &res);
    if (res < eps)
      std::cout << "    i = " << i << "  its = " << its << std::endl;
    else {
      std::cout << "    error: i=" << i << ", its=%i" << its << ", res=" << res
		<< ", u=" << u[i] << ", v=" << v[i] << ", w=" << w[i] << std::endl;
      return 1;
    }
  }

  // 7. stop timer
  double ftime = MPI_Wtime();
  double runtime = ftime - stime;

  // 8. output solution time
  std::cout << "     runtime = " << runtime << std::endl;

  // 9. free temperature and solution arrays
  delete[] T;
  delete[] u;
  delete[] v;
  delete[] w;

  // finalize MPI
  ierr = MPI_Finalize();

  return 0;
} // end main
