! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================

subroutine tridiag_matvec(n,A,x,r)
  !-----------------------------------------------------------------
  ! Description:
  !    Computes the matrix-vector product r = A*x, where A is a
  !    tridiagonal matrix, with the three diagonals stored in the
  !    columns of A.
  !
  !    The alignment of these arrays gives the system row i:
  !        r(i) = A(1,i)*x(i-1) + A(2,i)*x(i) + A(3,i)*x(i+1)
  !
  ! Arguments:
  !    n - integer (input), system size
  !    A - double (input), diagonals of the matrix
  !    x - double (input), input vector
  !    r - double (output), output vector
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none
  integer, intent(in) :: n
  double precision, intent(in)  :: A(n,3), x(n)
  double precision, intent(out) :: r(n)
  integer :: i

  !======= Internals ============

  ! compute the first entry of r
  r(1) = A(2,1)*x(1) + A(3,1)*x(2)

  ! compute the middle entries of r
  do i = 2,n-1
     r(i) = A(1,i)*x(i-1) + A(2,i)*x(i) + A(3,i)*x(i+1)
  enddo

  ! compute the last entry of r
  r(n) = A(1,n)*x(n-1) + A(2,n)*x(n)

  return

end subroutine tridiag_matvec
!=================================================================
