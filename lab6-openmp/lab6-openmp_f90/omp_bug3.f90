! -*- Mode: Fortran90; -*-
!=================================================================

program BUG4
  !-----------------------------------------------------------------
  ! FILE: omp_bug4.f90
  ! DESCRIPTION:
  !   This very simple program causes a segmentation fault.
  ! AUTHOR: Blaise Barney  01/09/04
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: NThreads, TID, i, j
  integer, parameter :: N=1048
  integer, external :: omp_get_num_threads, omp_get_thread_num
  real*8 :: A(N,N)

  !======= Internals ============

  ! Fork a team of threads with explicit variable scoping
  !$omp parallel shared(NThreads) private(i,j,TID,A)

  ! Obtain/print thread info
  TID = omp_get_thread_num()
  if (TID == 0) then
     NThreads = omp_get_num_threads()
     print *, 'Number of threads = ', NThreads
  end if
  print *, 'Thread',TID,' starting...'

  ! Each thread works on its own private copy of the array
  do i=1,N
     do j=1,N
        A(j,i) = 1.d0*(TID + i + j)
     end do
  end do

  ! For confirmation
  print *, 'Thread',TID,'done. Last element=',A(N,N)

  ! All threads join master thread and disband
  !$omp end parallel

end program BUG4
!=================================================================

