! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================

program Driver
  !-----------------------------------------------------------------
  ! Description:
  !    Allocates and fills the multi-dimensional vectors x, y, z
  !    with random numbers.
  !    Calls a set of vector routines.
  !    Writes the total time taken for all vector routines.
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer, parameter :: l = 100
  integer, parameter :: m = 100
  integer, parameter :: n = 100
  integer :: i, ierr
  double precision :: x(l,m,n), y(l,m,n), z(l,m,n)
  double precision :: a, etime, stime

  !======= Internals ============

  ! fill in vectors x and y
  call random_number(x)
  call random_number(y)

  ! get the start time (in seconds)
  call get_time(stime)

  ! call the vector routines a number of times
  do i = 1,100
     call vector_sum(l,m,n,x,y,z)
     call vector_scale(l,m,n,2.d0,x,z)
     call vector_infnorm(l,m,n,z,a)
     call vector_dotprod(l,m,n,x,y,a)
  enddo

  ! get the end time (in seconds)
  call get_time(etime)

  ! output the total time required for the operations (in seconds)
  if (etime < stime) then
     etime = etime + 24*3.6d3
  endif
  write(*,'(A,es16.9)') ' Result from computation =',a
  write(*,'(A,es16.9)') ' Total run time =',etime-stime

end program Driver
!=================================================================
