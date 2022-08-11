! -*- Mode: Fortran90; -*-
!=================================================================

program WorkShare2
  !-----------------------------------------------------------------
  ! FILE: omp_workshare2.f90
  ! DESCRIPTION:
  !   OpenMP Example - Sections Work-sharing
  !   In this example, the OpenMP SECTION directive is used to 
  !   assign different array operations to each thread that 
  !   executes a SECTION. 
  ! AUTHOR: Blaise Barney  5/99, 07/16/07
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: I, NThreads, TID, omp_get_num_threads, omp_get_thread_num
  integer, parameter :: N=50
  real :: A(N), B(N), C(N), D(N)

  !======= Internals ============

  ! Some initializations
  do I = 1, N
     A(I) = I * 1.5
     B(I) = I + 22.35
     C(N) = 0.0
     D(N) = 0.0
  enddo

  !$omp parallel shared(A,B,C,D,NThreads), private(I,TID)
  TID = omp_get_thread_num()
  if (TID .eq. 0) then
     NThreads = omp_get_num_threads()
     print *, 'Number of threads =', NThreads
  end if
  print *, 'Thread',TID,' starting...'

  !$omp sections

  !$omp section
  print *, 'Thread',TID,' doing section 1'
  do I = 1, N
     C(I) = A(I) + B(I)
     print *, ' Thread',TID,': C(',I,')=',C(I)
  enddo

  !$omp section
  print *, 'Thread',TID,' doing section 2'
  do I = 1, N
     D(I) = A(I) * B(I)
     print *, ' Thread',TID,': D(',I,')=',D(I)
  enddo

  !$omp end sections nowait

  print *, 'Thread',TID,' done.'

  !$omp end parallel

end program WorkShare2
!=================================================================
