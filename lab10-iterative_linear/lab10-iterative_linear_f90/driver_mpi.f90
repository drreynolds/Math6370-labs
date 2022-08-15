! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU, Mathematics
! Math 4370 / 6370
!=================================================================

program Iterative_main
  !-----------------------------------------------------------------
  ! Description:
  !   We set up and solve the linear system
  !           (I + gamma*L)u = r,
  !   where L is a standard 1D Laplace operator, r is a given
  !   right-hand side, and u is the solution, using a parallelized
  !   Jacobi iterative solver.
  !
  !   Requires three input arguments: gamma, delta, and n
  !   (the size the global domain).
  !-----------------------------------------------------------------
  !======= Inclusions ===========
  use mpi

  !======= Declarations =========
  implicit none

  double precision, allocatable :: u(:), r(:), a(:), b(:), c(:), res(:)
  double precision :: gamma, delta
  integer :: local_N, global_N, k, my_id, nprocs, ierr, comm
  integer :: maxiter, iters
  double precision :: stime, ftime, err2norm
  namelist /inputs/ gamma, delta, global_N

  !======= Internals ============

  ! intialize MPI
  call MPI_Init(ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Init'
     stop
  endif
  comm = MPI_COMM_WORLD
  call MPI_Comm_size(comm, nprocs, ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Comm_size'
     call MPI_Abort(comm, 1, ierr)
  endif
  call MPI_Comm_rank(comm, my_id, ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Comm_rank'
     call MPI_Abort(comm, 1, ierr)
  endif

  ! root gets problem information from input file
  if (my_id == 0) then
     open(100,file='input.txt',form='formatted')
     read(100,inputs)
     close(100)
  end if

  ! root node broadcasts information to other procs
  call MPI_Bcast(gamma, 1, MPI_REAL8, 0, comm, ierr)
  if (ierr /= 0) then
     print *, ' error in MPI_Bcast'
     call MPI_Abort(comm, 1, ierr);
  endif
  call MPI_Bcast(delta, 1, MPI_REAL8, 0, comm, ierr)
  if (ierr /= 0) then
     print *, ' error in MPI_Bcast'
     call MPI_Abort(comm, 1, ierr);
  endif
  call MPI_Bcast(global_N, 1, MPI_INTEGER, 0, comm, ierr)
  if (ierr /= 0) then
     print *, ' error in MPI_Bcast'
     call MPI_Abort(comm, 1, ierr);
  endif

  ! set local mesh sizes (last process takes up remainder)
  local_N = global_N/nprocs
  if ((my_id == nprocs-1) .and. (local_N*nprocs /= global_N)) then
     local_N = global_N - local_N*(nprocs-1)
  end if

  ! root outputs some information to screen
  if (my_id == 0) then
       print *,'iterative test with ',nprocs,' processors'
       print *,'    gamma = ',gamma
       print *,'    linear solver tolerance delta = ',delta
       print *,'    global problem size N = ',global_N
       print *,'    local problem sizes n = ',local_N
  endif

  ! Allocate vector memory
  allocate(u(local_N), r(local_N), a(local_N), b(local_N), &
           c(local_N), res(local_N), stat=ierr)
  if (ierr /= 0) then
     write(0,*) 'iterative test error: proc ',my_id,&
          ', failed to allocate array data'
     call MPI_Abort(comm, 1, ierr)
  end if

  ! Set up matrix arrays, right-hand side, and initial solution guess
  do k=1,local_N
     u(k) =  0.d0
     r(k) =  1.d0
     a(k) = -gamma
     b(k) =  1.d0+gamma*2.d0
     c(k) = -gamma
  end do

  ! Adjust a, c arrays if we are at either end of domain
  if (my_id == 0)         a(1) = 0.d0
  if (my_id == nprocs-1)  c(local_N) = 0.d0

  ! check linear residual
  call linresid(a, b, c, u, r, res, err2norm, local_N, comm, ierr)
  if (ierr /= 0) then
     write(0,*) 'iterative test error: linresid failed'
     call MPI_Abort(comm, 1, ierr)
  end if
  if (my_id == 0)  print *,' initial residual: ||T*u-r||_2 = ',err2norm

  ! Wait until all procs have caught up
  call MPI_Barrier(comm, ierr)
  if (ierr /= 0) then
     write(0,*) 'iterative test error: failed MPI_Barrier'
     call MPI_Abort(comm, 1, ierr)
  end if

  ! Solve system, get timing information
  maxiter = 10000
  stime = MPI_Wtime()
  call jacobi_solve(a, b, c, u, r, res, local_N, &
                    delta, maxiter, iters, comm, ierr)
  if (ierr /= 0) then
     print *,'iterative test error: jacobi_solve failed'
     call MPI_Abort(comm, 1, ierr)
  end if
  ftime = MPI_Wtime()
  if (my_id == 0) then
     print *,' converged in ',iters,' iterations at delta = ',delta
     print *,' solution time: ',ftime-stime,' seconds'
  end if

  ! check linear residual
  call linresid(a, b, c, u, r, res, err2norm, local_N, comm, ierr)
  if (ierr /= 0) then
     print *,'iterative test error: linresid failed'
     call MPI_Abort(comm, 1, ierr)
  end if
  if (my_id == 0)  print *,' final residual: ||T*u-r||_2 = ',err2norm


  ! Free matrix/solver memory
  deallocate(u,r,a,b,c,res)

  ! finalize MPI
  call MPI_Finalize(ierr)

end program Iterative_main
!=================================================================
