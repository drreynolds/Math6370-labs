! -*- Mode: Fortran90; -*-
!=================================================================

program Reduction
  !-----------------------------------------------------------------
  ! FILE: omp_reduction.f90
  ! DESCRIPTION:
  !   OpenMP Example - Combined Parallel Loop Reduction
  !   This example demonstrates a sum reduction within a combined 
  !   parallel loop construct.  Notice that default data element 
  !   scoping is assumed - there are no clauses specifying shared 
  !   or private variables.  OpenMP will automatically make loop 
  !   index variables private within team threads, and global 
  !   variables shared.
  ! AUTHOR: Blaise Barney  5/99
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: I, N
  real :: A(100), B(100), SUM

  !======= Internals ============

  ! Some initializations
  N = 100
  do I = 1, N
     A(I) = I * 1.0
     B(I) = A(I)
  enddo
  SUM = 0.0

  !$omp parallel do reduction(+:SUM)
  do I = 1, N
     SUM = SUM + (A(I) * B(I))
  enddo
  !$omp end parallel do

  print *, '   Sum = ', SUM

end program Reduction
!=================================================================
