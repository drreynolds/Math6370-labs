/* Daniel R. Reynolds
   SMU Mathematics
   Math 4370 / 6370 */

// Inclusions
#include <iostream>
#include <cmath>
#include "mpi.h"


// Description: calculates the linear residual and its averaged 2-norm (WRMS)
int linresid(double *a, double *b, double *c, double *u, double *r,
	     double *res, double &norm2, int loc_N, MPI_Comm comm) {

  // Get MPI parallelism information from comm
  int nprocs, my_id;
  if ( MPI_Comm_size(comm, &nprocs) != MPI_SUCCESS) {
    std::cerr << "linresid error in MPI_Comm_size\n";
    return 1;
  }
  if ( MPI_Comm_rank(comm, &my_id) != MPI_SUCCESS) {
    std::cerr << "linresid error in MPI_Comm_rank\n";
    return 1;
  }

  // initialize send/receive values
  double s_l = u[0];
  double s_r = u[loc_N-1];
  double u_l = 0.0;
  double u_r = 0.0;

  // phase 1: even procs exchange to right, odd ones exchange to left
  MPI_Status status;
  if (my_id%2 == 0) {

    if (my_id != nprocs-1) {   // check we're not last

      // send to right w/ tag 100
      if ( MPI_Send(&s_r, 1, MPI_DOUBLE, my_id+1, 100, comm) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Send\n";
        return 1;
      }

      // recv from right w/ tag 101
      if ( MPI_Recv(&u_r, 1, MPI_DOUBLE, my_id+1, 101, comm, &status) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Recv\n";
        return 1;
      }
    }

  } else {

    if (my_id != 0) {          // check we're not first

      // recv from left w/ tag 100
      if ( MPI_Recv(&u_l, 1, MPI_DOUBLE, my_id-1, 100, comm, &status) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Recv\n";
        return 1;
      }

      // send to left w/ tag 101
      if ( MPI_Send(&s_l, 1, MPI_DOUBLE, my_id-1, 101, comm) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Send\n";
        return 1;
      }
    }

  } // if my_id%2

  // phase 2: even procs exchange to left, odd ones exchange to right
  if (my_id%2 == 1) {

    if (my_id != nprocs-1) {   // check we're not last

      // send to right w/ tag 102
      if ( MPI_Send(&s_r, 1, MPI_DOUBLE, my_id+1, 102, comm) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Send\n";
        return 1;
      }

      // recv from right w/ tag 103
      if ( MPI_Recv(&u_r, 1, MPI_DOUBLE, my_id+1, 103, comm, &status) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Recv\n";
        return 1;
      }
    }

  } else {

    if (my_id != 0) {          // check we're not first

      // recv from left w/ tag 102
      if ( MPI_Recv(&u_l, 1, MPI_DOUBLE, my_id-1, 102, comm, &status) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Recv\n";
        return 1;
      }

      // send to left w/ tag 103
      if ( MPI_Send(&s_l, 1, MPI_DOUBLE, my_id-1, 103, comm) != MPI_SUCCESS) {
        std::cerr << "linresid error in MPI_Send\n";
        return 1;
      }
    }

  } // if my_id%2

  // compute linear residual at left of subdomain
  res[0] = a[0]*u_l + b[0]*u[0] + c[0]*u[1] - r[0];
  norm2 = res[0]*res[0];

  // compute linear residual in interior of subdomain
  int k;
  for (k=1; k<loc_N-1; k++) {
    res[k] = a[k]*u[k-1] + b[k]*u[k] + c[k]*u[k+1] - r[k];
    norm2 += res[k]*res[k];
  }

  // compute linear residual at right end of subdomain
  k = loc_N-1;
  res[k] = a[k]*u[k-1] + b[k]*u[k]+ c[k]*u_r - r[k];
  norm2 += res[k]*res[k];

  // combine local 2-norms into global averaged 2-norm
  // (this assumes that each process has the same loc_N)
  double tmp;
  if ( MPI_Allreduce(&norm2, &tmp, 1, MPI_DOUBLE, MPI_SUM, comm) != MPI_SUCCESS) {
    std::cerr << "linresid error in MPI_Allreduce\n";
    return 1;
  }
  norm2 = sqrt(tmp)/loc_N/nprocs;

  return 0;
} // end linresid
