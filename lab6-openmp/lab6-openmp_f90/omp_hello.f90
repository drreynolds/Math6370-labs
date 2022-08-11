! -*- Mode: Fortran90; -*-
!=================================================================

program Hello
  !-----------------------------------------------------------------
  ! FILE: omp_hello.f90
  ! DESCRIPTION:
  !   OpenMP Example - Hello World
  !   In this simple example, the master thread forks a parallel 
  !   region.  All threads in the team obtain their unique thread 
  !   number and print it.  The master thread only prints the total 
  !   number of threads.  Two OpenMP library routines are used to 
  !   obtain the number of threads and each thread's number.
  ! AUTHOR: Blaise Barney  5/99
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: NThreads, TID
  integer, external :: omp_get_num_threads, omp_get_thread_num

  !======= Internals ============

  ! Fork a team of threads giving them their own copies of variables
  !$omp parallel private(NThreads, TID)

  ! Obtain thread number
  TID = omp_get_thread_num()
  print *, 'Hello World from thread = ', TID

  ! Only master thread does this
  if (TID == 0) then
     NThreads = omp_get_num_threads()
     print *, 'Number of threads = ', NThreads
  end if

  ! All threads join master thread and disband
  !$omp end parallel

end program Hello
!=================================================================
