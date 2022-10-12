! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================

program Driver
  !-----------------------------------------------------------------
  ! Description:
  !    Sets up the tridiagonal matrix A and vector x.  The diagonals
  !    of A are held in three vectors of length n, all stored in the
  !    three columns of A.
  !    Computes matrix-vector product r = A*x.
  !    Outputs ||r||_2 to the screen.
  !    Writes r to the file 'r.txt'
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer, parameter :: n = 1000
  integer :: i
  double precision :: A(3,n), x(n), r(n)
  double precision :: norm2

  !======= Internals ============

  ! fill in diagonals of A and vector x
  do i=1,n
     A(1,i) = -1.d0
     A(2,i) = 2.d0
     A(3,i) = -1.d0
     x(i) = 1.d0
  enddo

  ! call the tridiagonal matrix-vector product
  call tridiag_matvec(n,A,x,r)

  ! output the 2-norm of the result to screen, and the result to file
  norm2 = 0.d0
  do i = 1,n
     norm2 = norm2 + r(i)**2
  enddo
  write(*,'(A,es16.9)') ' 2-norm of product =',sqrt(norm2)
  open(101,file='r.txt',form='formatted')
  do i = 1,n
     write(101,'(es22.15)') r(i)
  enddo
  close(101)

end program Driver
!=================================================================
