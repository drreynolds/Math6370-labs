/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Inclusions */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/* Prototypes */
void vector_sum(int, int, int, double***, double***, double***);
void vector_scale(int, int, int, double, double***, double***);
double vector_infnorm(int, int, int, double***);
double vector_dotprod(int, int, int, double***, double***);


/* Allocates and fills the multi-dimensional vectors x, y, z
   with random numbers.
   Calls a set of vector routines.
   Writes the total time taken for all vector routines. */
int main(int argc, char* argv[]) {

  /* declarations */
  int i, j, k, l, m, n;
  double ***x, ***y, ***z, a, runtime;
  time_t stime, ftime;

  /* set problem parameters */
  l = 100;
  m = 100;
  n = 100;

  /* allocate 3D data arrays */
  x = malloc( l * sizeof(double**) );
  y = malloc( l * sizeof(double**) );
  z = malloc( l * sizeof(double**) );
  for (i=0; i<l; i++)  x[i] = malloc( m * sizeof(double*) );
  for (i=0; i<l; i++)  y[i] = malloc( m * sizeof(double*) );
  for (i=0; i<l; i++)  z[i] = malloc( m * sizeof(double*) );
  for (i=0; i<l; i++)
    for (j=0; j<m; j++) x[i][j] = malloc( n * sizeof(double) );
  for (i=0; i<l; i++)
    for (j=0; j<m; j++) y[i][j] = malloc( n * sizeof(double) );
  for (i=0; i<l; i++)
    for (j=0; j<m; j++) z[i][j] = malloc( n * sizeof(double) );

  /* fill in vectors x and y */
  for (i=0; i<l; i++)
    for (j=0; j<m; j++)
      for (k=0; k<n; k++)
	x[i][j][k] = random() / (pow(2.0,31.0) - 1.0);
  for (i=0; i<l; i++)
    for (j=0; j<m; j++)
      for (k=0; k<n; k++)
	y[i][j][k] = random() / (pow(2.0,31.0) - 1.0);

  /* start timer */
  stime = time(NULL);

  /* call the vector routines a number of times */
  for (i=0; i<100; i++) {
    vector_sum(l,m,n,x,y,z);
    vector_scale(l,m,n,2.0,x,z);
    a = vector_infnorm(l,m,n,z);
    a = vector_dotprod(l,m,n,x,y);
  }

  /* stop timer */
  ftime = time(NULL);
  runtime = ((double) (ftime - stime));

  /* output solution time */
  printf(" Result from computation = %.12e\n",a);
  printf(" Total run time = %.12e\n",runtime);

  /* free data arrays */
  for (i=0; i<l; i++) {
    for (j=0; j<m; j++) {
      free(x[i][j]);
      free(y[i][j]);
      free(z[i][j]);
    }
    free(x[i]);
    free(y[i]);
    free(z[i]);
  }
  free(x);
  free(y);
  free(z);

} /* end main */
