! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================

program Iterative_main
  !-----------------------------------------------------------------
  ! Description:
  !   We set up and solve the linear system
  !           (I + gamma*L)u = r,
  !   where L is a standard 1D Laplace operator, r is a given
  !   right-hand side, and u is the solution, using a
  !   Jacobi iterative solver.
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  double precision, allocatable :: u(:), r(:), a(:), b(:), c(:), res(:)
  double precision :: gamma, delta
  integer :: global_N, k, ierr
  integer :: maxiter, iters
  double precision :: stime, ftime, err2norm
  namelist /inputs/ gamma, delta, global_N

  !======= Internals ============

  ! get problem information from input file
  open(100,file='input.txt',form='formatted')
  read(100,inputs)
  close(100)

  ! output some information to screen
  print *,'iterative test'
  print *,'    gamma = ',gamma
  print *,'    linear solver tolerance delta = ',delta
  print *,'    problem size N = ',global_N

  ! Allocate vector memory
  allocate(u(global_N), r(global_N), a(global_N), b(global_N), &
           c(global_N), res(global_N), stat=ierr)
  if (ierr /= 0) then
     print *,'iterative test error: failed to allocate array data'
     stop
  end if

  ! Set up matrix arrays, right-hand side, and initial solution guess
  do k=1,global_N
     u(k) =  0.d0
     r(k) =  1.d0
     a(k) = -gamma
     b(k) =  1.d0+gamma*2.d0
     c(k) = -gamma
  end do

  ! Adjust a, c arrays at ends of domain
  a(1) = 0.d0
  c(global_N) = 0.d0

  ! check linear residual
  call linresid(a, b, c, u, r, res, err2norm, global_N, ierr)
  if (ierr /= 0) then
     print *,'iterative test error: linresid failed'
     stop
  end if
  print *,' initial residual: ||T*u-r||_2 = ',err2norm

  ! Solve system, get timing information
  maxiter = 10000
  call get_time(stime)
  call jacobi_solve(a, b, c, u, r, res, global_N, &
                    delta, maxiter, iters, ierr)
  if (ierr /= 0) then
     print *,'iterative test error: jacobi_solve failed'
     stop
  end if
  call get_time(ftime)
  print *,' converged in ',iters,' iterations at delta = ',delta
  print *,' solution time: ',ftime-stime,' seconds'

  ! check linear residual
  call linresid(a, b, c, u, r, res, err2norm, global_N, ierr)
  if (ierr /= 0) then
     print *,'iterative test error: linresid failed'
     stop
  end if
  print *,' final residual: ||T*u-r||_2 = ',err2norm


  ! Free matrix/solver memory
  deallocate(u,r,a,b,c,res)

end program Iterative_main
!=================================================================
