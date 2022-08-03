/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370
*/


// Description: computes the pointwise sum u+v=w for vectors of length n
void vector_sum(int n, double* u, double* v, double* w) {

  for (int i=0; i<n; i++)
    w[i] = u[i] + v[i];

  return;
}
