! -*- Mode: Fortran90; -*-
!=================================================================

program BUG5
  !-----------------------------------------------------------------
  ! FILE: omp_bug5.f90
  ! DESCRIPTION:
  !   Using SECTIONS, two threads initialize their own array and 
  !   then add it to the other's array, however a deadlock occurs.
  ! AUTHOR: Blaise Barney  01/09/04
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer*8 :: LOCKA, LOCKB
  integer :: NThreads, TID, i
  integer, parameter :: N=1000000
  integer, external :: omp_get_num_threads, omp_get_thread_num
  real :: A(N), B(N)
  real, parameter :: PI=3.1415926535
  real, parameter :: DELTA=.01415926535

  !======= Internals ============

  ! Initialize the locks
  call omp_init_lock(LOCKA)
  call omp_init_lock(LOCKB)

  ! Fork a team of threads giving them their own copies of variables
  !$omp parallel shared(A, B, NThreads, LOCKA, LOCKB) private(TID)

  ! Obtain thread number and number of threads
  TID = omp_get_thread_num()

  !$omp master
  NThreads = omp_get_num_threads()
  print *, 'Number of threads = ', NThreads
  !$omp end master

  print *, 'Thread', TID, 'starting...'

  !$omp barrier

  !$omp sections

  !$omp section
  print *, 'Thread',TID,' initializing A()'
  call omp_set_lock(LOCKA)
  do i = 1, N
     A(i) = i * DELTA
  enddo

  call omp_set_lock(LOCKB)
  print *, 'Thread',TID,' adding A() to B()'
  do i = 1, N
     B(i) = B(i) + A(i)
  enddo

  call omp_unset_lock(LOCKB)
  call omp_unset_lock(LOCKA)

  !$omp section
  print *, 'Thread',TID,' initializing B()'
  call omp_set_lock(LOCKB)
  do i = 1, N
     B(i) = i * PI
  enddo

  call omp_set_lock(LOCKA)
  print *, 'Thread',TID,' adding B() to A()'
  do i = 1, N
     A(i) = A(i) + B(i)
  enddo

  call omp_unset_lock(LOCKA)
  call omp_unset_lock(LOCKB)

  !$omp end sections nowait

  print *, 'Thread',TID,' done.'

  !$omp end parallel

end program BUG5
!=================================================================
