/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370
*/

#include <stdlib.h>
#include <iostream>
#include <iomanip>

// prototypes
double one_norm(int n, double* u);
void vector_sum(int n, double* u, double* v, double* w);
void vector_difference(int n, double* u, double* v, double* w);
void vector_product(int n, double* u, double* v, double* w);


// Description: Simple example program that calls multiple functions.
int main() {

  // declare vectors u,v,w; fill u and v and output to screen
  double u[10], v[10], w[10];
  for (int i=0; i<10; i++) {
    u[i] = (double) i;
    v[i] = (double) (10-i);
  }
  std::cout << "   u = ";
  for (int i=0; i<10; i++)  std::cout << u[i] << " ";
  std::cout << std::endl;
  std::cout << "   v = ";
  for (int i=0; i<10; i++)  std::cout << v[i] << " ";
  std::cout << std::endl;

  // call the one-norm function and output result to screen
  double res = one_norm(10,u);
  std::cout << "   one norm of u = " << res << std::endl;

  // call the vector sum subroutine and output result to screen
  vector_sum(10,u,v,w);
  std::cout << "   sum = ";
  for (int i=0; i<10; i++)  std::cout << w[i] << " ";
  std::cout << std::endl;

  // call the vector difference subroutine and output result to screen
  vector_difference(10,u,v,w);
  std::cout << "   difference = ";
  for (int i=0; i<10; i++)  std::cout << w[i] << " ";
  std::cout << std::endl;

  // call the vector product subroutine and output result to screen
  vector_product(10,u,v,w);
  std::cout << "   product = ";
  for (int i=0; i<10; i++)  std::cout << w[i] << " ";
  std::cout << std::endl;

  return 0;
}
