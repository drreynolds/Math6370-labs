! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU, Mathematics
! Math 4370 / 6370
!=================================================================

program ChemicalEquilibrium
  !-----------------------------------------------------------------
  ! Description:
  !    Computes the equilibrium chemical densities at a number of
  !    spatial locations, given a (random) background temperature
  !    field.  The chemical rate equations and solution strategy
  !    are in the subroutine chem_solver, which is called at every
  !    spatial location.
  !-----------------------------------------------------------------
  !======= Inclusions ===========
  use mpi
  implicit none

  !======= Interfaces ===========
  interface
     subroutine chem_solver(T,u,v,w,lam,eps,maxit,its,res)
       integer, intent(in) :: maxit
       integer, intent(out) :: its
       double precision, intent(in) :: T, lam, eps
       double precision, intent(out) :: res
       double precision, intent(inout) :: u, v, w
     end subroutine chem_solver
  end interface

  !======= Declarations =========
  integer :: n, i, maxit, its, ierr
  double precision, allocatable :: T(:), u(:), v(:), w(:)
  double precision :: lam, eps, res, stime, ftime

  !======= Internals ============

  ! initialize MPI
  call MPI_Init(ierr)
  if (ierr /= MPI_SUCCESS) then
     write(0,*) 'Error in calling MPI_Init'
     stop
  endif

  ! 1. set solver input parameters
  maxit = 1000000
  lam = 1.d-2
  eps = 1.d-10

  ! 2. input the number of intervals
  write(*,*) 'Enter the number of intervals (0 quits):'
  read(*,*) n
  if (n < 1) then
     stop
  endif

  ! 3. allocate temperature and solution arrays
  allocate(T(n),u(n),v(n),w(n))

  ! 4. set random temperature field, initial guesses at chemical densities
  call random_number(T)
  u = 0.35d0
  v = 0.1d0
  w = 0.5d0

  ! 5. start timer
  stime = MPI_Wtime()

  ! 6. call solver over n intervals
  do i=1,n,1
     call chem_solver(T(i),u(i),v(i),w(i),lam,eps,maxit,its,res)
     if (res < eps) then
        write(*,'(2(A,i8))') '    i =',i,',  its =',its
     else
        write(*,'(2(A,i8),4(A,es9.2))') '    error: i =',i,',  its =',its, &
             ',  res =',res,',  u =',u(i),',  v =',v(i),',  w =',w(i)
        stop
     endif
  enddo

  ! 7. stop timer
  ftime = MPI_Wtime()

  ! 8. output solution time
  write(*,*) '     runtime =',ftime-stime

  ! 9. free temperature and solution arrays
  deallocate(T,u,v,w)

  ! finalize MPI
  call MPI_Finalize(ierr)

end program ChemicalEquilibrium
!=================================================================
