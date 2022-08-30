/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>

// Set timer based on OpenMP availability
#ifdef _OPENMP
  #include "omp.h"
  #define TIMER omp_get_wtime()
#else
  #include <time.h>
  #define TIMER (1.0*clock()/CLOCKS_PER_SEC)
#endif


// Prototypes
inline double f(double a, double b) { return (a*exp(3.0*a)*sin(25.0*M_PI*b)); }


/* Main routine to approximate the 2D integral
      int_Omega f(x,y) dx dy
   where Omega is the unit square, [0,1]x[0,1].
   We use a composite Gaussian quadrature rule with 8 points
   per subinterval in each direction (64 points per sub-square),
   over sub-squares of fixed size 1/n x 1/n. */
int main(int argc, char* argv[]) {

  // declarations
  int n = 1000;
  int i, j, k, l;
  double F, h, x, y, a, b, stime, ftime, runtime;
  double F_true = 2.0/225.0/M_PI*(2.0*exp(3.0)+1.0);
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

  // start timer
  stime = TIMER;

  // set subinterval width
  h = 1.0 / n;

  // initialize result
  F = 0.0;

  // perform integration over n intervals in each direction
# pragma omp parallel for collapse(2) default(shared) private(i,j,k,l)
  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {

      // in each sub-square, evaluate at all 64 points and combine results
      for (k=0; k<nodes; k++) {
        for (l=0; l<nodes; l++) {

          // location of sub-square center
          x = h * (i + 0.5);
          y = h * (j + 0.5);

          // location of quadrature evaluation point
          a = x + 0.5*h*z[k];
          b = y + 0.5*h*z[l];

          // update numerical integral with contribution from point
          F += 0.25*h*h*w[k]*w[l]*f(a,b);

	}  // end l loop
      }  // end k loop
    }  // end j loop
  }  // end i loop

  // stop timer
  ftime = TIMER;
  runtime = ftime - stime;

  // output computed value and error
  std::cout << " computed F = " << std::setprecision(16) << F << "\n";
  std::cout << "     true F = " << F_true << "\n";
  std::cout << "      error = " << std::setprecision(5) << std::abs(F_true-F) << "\n";
  std::cout << "    runtime = " << runtime << "\n";

} // end main
