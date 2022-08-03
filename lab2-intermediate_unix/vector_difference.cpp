/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370
*/


// Description: computes the pointwise difference u-v=w for vectors of length n
void vector_difference(int n, double* u, double* v, double* w) {

  for (int i=0; i<n; i++)
    w[i] = u[i] - v[i];

  return;
}
