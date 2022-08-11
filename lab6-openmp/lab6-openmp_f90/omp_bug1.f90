! -*- Mode: Fortran90; -*-
!=================================================================

program Bug1
  !-----------------------------------------------------------------
  ! FILE: omp_bug1.f90
  ! DESCRIPTION:
  !   This example attempts to show use of the PARALLEL DO 
  !   construct.  However it will generate errors at compile time.  
  !   Try to determine what is causing the error.  See 
  !   omp_bug1fix.f90 for a corrected version.
  ! AUTHOR: Blaise Barney  5/99
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: TID, i, chunk
  integer, external :: omp_get_thread_num
  integer, parameter :: N = 50
  integer, parameter :: chunksize = 5
  real :: A(N), B(N), C(N)

  !======= Internals ============

  !     Some initializations
  do i = 1, N
     A(i) = i * 1.0
     B(i) = A(i)
  enddo
  chunk = chunksize

  !$omp parallel do shared(A,B,C,chunk) private(i,TID) &
  !$omp schedule(static,chunk)
  TID = omp_get_thread_num()
  do i = 1, N
     C(i) = A(i) + B(i)
     print *,'TID= ',TID,'i= ',i,'C(i)= ',C(i)
  enddo
  !$omp end parallel do

end program Bug1
!=================================================================
