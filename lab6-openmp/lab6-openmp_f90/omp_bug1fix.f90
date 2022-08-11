! -*- Mode: Fortran90; -*-
!=================================================================

program Bug1
  !-----------------------------------------------------------------
  ! FILE: omp_bug1fix.f90
  ! DESCRIPTION:
  !   This is a corrected version of the omp_bug1fix.f90 example. 
  !   Corrections include removing all statements between the 
  !   PARALLEL DO construct and the actual DO loop, and introducing 
  !   logic to preserve the ability to query a thread's id and 
  !   print it from inside the DO loop.
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
  character :: first_time

  !======= Internals ============

  ! Some initializations
  do i = 1, N
     A(i) = i * 1.0
     B(i) = A(i)
  enddo
  chunk = chunksize
  first_time = 'Y'

  !$omp parallel do shared(A,B,C,chunk) private(i,TID) &
  !$omp schedule(static,chunk) firstprivate(first_time) 
  do i = 1,N
     if (first_time == 'Y') then
        TID = omp_get_thread_num()
        first_time = 'N'
     endif
     C(i) = A(i) + B(i)
     print *,'TID= ',TID,'i= ',i,'C(i)= ',C(i)
  enddo
  !$omp end parallel do

end program Bug1
!=================================================================

