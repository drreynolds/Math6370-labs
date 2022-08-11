! -*- Mode: Fortran90; -*-
!=================================================================

program WorkShare1
  !-----------------------------------------------------------------
  ! FILE: omp_workshare1.f90
  ! DESCRIPTION:
  !   OpenMP Example - Loop Work-sharing 
  !   In this example, the iterations of a loop are scheduled 
  !   dynamically across the team of threads.  A thread will 
  !   perform chunk iterations at a time before being scheduled 
  !   for the next chunk of work.
  ! AUTHOR: Blaise Barney  5/99, 1/09/04
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: NThreads, TID, chunk, I 
  integer, external :: omp_get_num_threads, omp_get_thread_num
  integer, parameter :: N = 100
  integer, parameter :: chunksize = 10
  real :: A(N), B(N), C(N)

  !======= Internals ============

  ! Some initializations
  do I = 1, N
     A(I) = I * 1.0
     B(I) = A(I)
  enddo
  chunk = chunksize

  !$omp parallel shared(A,B,C,NThreads,chunk) private(I,TID)

  TID = omp_get_thread_num()
  if (TID .eq. 0) then
     NThreads = omp_get_num_threads()
     print *, 'Number of threads =', NThreads
  end if
  print *, 'Thread',TID,' starting...'

  !$omp do schedule(dynamic,chunk)
  do I = 1, N
     C(I) = A(I) + B(I)
     print *, ' Thread',TID,': C(',I,')=',C(I)
  enddo
  !$omp end do nowait

  print *, 'Thread',TID,' done.'

  !$omp end parallel

end program WorkShare1
!=================================================================
