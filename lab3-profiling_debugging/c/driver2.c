/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Inclusions */
#include <stdlib.h>   /* malloc(), free() */
#include <stdio.h>    /* printf() */
#include <math.h>     /* sqrt() */

/* Prototypes */
void tridiag_matvec(int, double**, double*, double*);


/* Sets up the tridiagonal matrix A and vector x.  The diagonals
   of A are held in three vectors of length n, all stored in the
   three columns of A.
   Computes matrix-vector product r = A*x.
   Outputs ||r||_2 to the screen.
   Writes r to the file 'r.txt'
 */
int main(int argc, char* argv[]) {

  /* declarations */
  int n, i;
  double *x, *r, **A, norm2;
  FILE *fid;

  /* set problem parameters */
  n = 1000;

  /* allocate data arrays */
  x = malloc( n * sizeof(double) );
  r = malloc( n * sizeof(double) );
  A = malloc( 3 * sizeof(double*) );
  for (i=0; i<3; i++)  A[i] = malloc( n * sizeof(double) );

  /* fill in diagonals of A and vector x */
  for (i=0; i<n; i++) {
    A[0][i] = -1.0;
    A[1][i] =  2.0;
    A[2][i] = -1.0;
    x[i] = 1.0;
  }

  /* call the tridiagonal matrix-vector product */
  tridiag_matvec(n,A,x,r);

  /* output the 2-norm of the result to screen, and the result to file */
  norm2 = 0.0;
  for (i=0; i<n; i++)  norm2 += r[i]*r[i];
  printf(" 2-norm of product = %.12e\n", sqrt(norm2));
  fid = fopen("r.txt","w");
  for (i=0; i<n; i++)  fprintf(fid,"%.12e\n", r[i]);
  fclose(fid);

  /* free data arrays */
  for (i=0; i<3; i++)  free(A[i]);
  free(A);
  free(x);
  free(r);

} /* end main */
