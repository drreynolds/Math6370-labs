/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

/* Description:
      Computes the matrix-vector product r = A*x, where A is a
      tridiagonal matrix, with the three diagonals stored in the
      columns of A.

      The alignment of these arrays gives the system row i:
          r(i) = A(1,i)*x(i-1) + A(2,i)*x(i) + A(3,i)*x(i+1)

   Arguments:
      n - integer (input), system size
      A - double (input), diagonals of the matrix
      x - double (input), input vector
      r - double (output), output vector
*/
void tridiag_matvec(int n, double **A, double *x, double *r) {

  /* declarations */
  int i;

  /* compute the first entry of r */
  r[0] = A[0][1]*x[0] + A[0][2]*x[1];

  /* loop to compute the middle entries of r */
  for (i=1; i<n-1; i++)
    r[i] = A[i][0]*x[i-1] + A[i][1]*x[i] + A[i][2]*x[i+1];

  /* compute the last entry of r */
  r[n-1] = A[n-1][0]*x[n-2] + A[n-1][1]*x[n-1];

} /* end tridiag_matvec */
