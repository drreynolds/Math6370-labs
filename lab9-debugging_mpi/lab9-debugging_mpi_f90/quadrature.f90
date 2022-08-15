! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================

program Quadrature
  !-----------------------------------------------------------------
  ! Description:
  !    Routine to approximate the 2D integral
  !       int_Omega f(x,y) dx dy
  !    where Omega is the unit square, [0,1]x[0,1].
  !    We use a composite Gaussian quadrature rule with 8 points
  !    per subinterval in each direction (64 points per sub-square),
  !    over sub-squares of fixed size 1/n x 1/n
  !-----------------------------------------------------------------
  !======= Inclusions ===========
  use mpi

  !======= Declarations =========
  implicit none

  integer,          parameter :: n = 1000
  integer,          parameter :: nodes = 8
  double precision, parameter :: PI=3.14159265358979323846d0
  integer                     :: i, j, k, l, is, ie, js, je, ierr, numprocs, myid
  double precision            :: fn, F, F_all, F_true, h, x, y, a, b
  double precision            :: stime, ftime, runtime
  double precision            :: z(nodes), w(nodes)

  !======= Internals ============

  ! set the integrand function
  fn(a,b) = a*exp(3.d0*a)*sin(25.d0*PI*b)

  ! intialize MPI
  call MPI_Init(ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Init =',ierr
     stop
  endif
  call MPI_Comm_size(MPI_COMM_WORLD, numprocs, ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Comm_size =',ierr
     call MPI_Abort(MPI_COMM_WORLD, 1, ierr)
  endif
  call MPI_Comm_rank(MPI_COMM_WORLD, myid, ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Comm_rank =',ierr
     call MPI_Abort(MPI_COMM_WORLD, 1, ierr)
  endif

  ! set the true integral value
  F_true = 2.d0/225.d0/PI*(2.d0*exp(3.d0)+1.d0)

  ! set the quadrature data
  z = (/ -0.18343464249564980493d0, &
          0.18343464249564980493d0, &
         -0.52553240991632898581d0, &
          0.52553240991632898581d0, &
         -0.79666647741362673959d0, &
          0.79666647741362673959d0, &
         -0.96028985649753623168d0, &
          0.96028985649753623168d0 /)
  w = (/ 0.36268378337836198296d0, &
         0.36268378337836198296d0, &
         0.31370664587788728733d0, &
         0.31370664587788728733d0, &
         0.22238103445337447054d0, &
         0.22238103445337447054d0, &
         0.10122853629037625915d0, &
         0.10122853629037625915d0 /)

  ! root outputs parallelism information to screen
  if (myid == 0)  print *, 'Running with ',numprocs,' MPI tasks'

  ! start timer
  stime = MPI_Wtime()

  ! set subinterval width
  h = 1.d0 / n

  ! determine this processor's sub-region of the 2D region
  is = floor(1.d0*n/numprocs)*myid + 1
  ie = floor(1.d0*n/numprocs)*(myid+1)
  if (myid == numprocs-1) then
     ie = n
  endif

  js = floor(1.d0*n/numprocs)*myid + 1
  je = floor(1.d0*n/numprocs)*(myid+1)
  if (myid == numprocs-1) then
     je = n
  endif

  ! initialize local result
  F = 0.d0

  ! perform integration over n intervals in each direction
  do i=is,ie
     do j=js,je

        ! in each sub-square, evaluate at all 64 points and combine results
        do k=1,nodes
           do l=1,nodes

              ! location of sub-square center
              x = h * (i - 0.5d0)
              y = h * (j - 0.5d0)

              ! location of quadrature evaluation point
              a = x + 0.5d0*h*z(k)
              b = y + 0.5d0*h*z(l)

              ! update numerical integral with contribution from point
              F = F + 0.25d0*h*h*w(k)*w(l)*fn(a,b)

           enddo  ! end l loop
        enddo  ! end k loop
     enddo  ! end j loop
  enddo  ! end i loop

  ! perform reduction to get result on root process
  call MPI_Reduce(F, F_all, 1, MPI_REAL8, MPI_SUM, 0, MPI_COMM_WORLD, ierr)
  if (ierr /= 0) then
     write(0,*) ' error in MPI_Reduce =',ierr
     call MPI_Abort(MPI_COMM_WORLD, 1, ierr)
  endif

  ! stop timer
  ftime = MPI_Wtime()
  runtime = ftime - stime

  ! root outputs computed value and error
  if (myid == 0) then
    print '(A,es23.16)', ' computed F =', F_all
    print '(A,es23.16)', '     true F =', F_true
    print '(A,es12.5)', '      error =', abs(F_true-F_all)
    print '(A,f8.5)', '    runtime =', runtime
  endif

  ! finalize MPI
  call MPI_Finalize(ierr)

end program Quadrature
!=================================================================
