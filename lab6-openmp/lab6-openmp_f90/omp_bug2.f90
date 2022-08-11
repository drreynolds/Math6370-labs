! -*- Mode: Fortran90; -*-
!=================================================================

program BUG2
  !-----------------------------------------------------------------
  ! FILE: omp_bug2.f90
  ! DESCRIPTION:
  !   Another OpenMP program with a bug
  ! AUTHOR: Blaise Barney  1/7/04
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: NThreads, i, TID
  real*8 :: total
  integer, external :: omp_get_num_threads, omp_get_thread_num

  !======= Internals ============

  ! Spawn parallel region
  !$omp parallel

  ! Obtain thread number
  TID = omp_get_thread_num()

  ! Only master thread does this
  if (TID == 0) then
     NThreads = omp_get_num_threads()
     print *, 'Number of threads = ', NThreads
  end if
  print *, 'Thread ',TID,'is starting...'

  !$omp barrier

  ! Do some work
  total = 0.0
  !$omp do schedule(dynamic,10)
  do i=1,1000000
     total = total + i*1.d0
  end do

  write(*,*) 'Thread',TID,' is done! Total= ',total
  !$omp end parallel

end program BUG2
!=================================================================
