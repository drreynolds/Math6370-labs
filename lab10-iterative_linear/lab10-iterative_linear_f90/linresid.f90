! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================

subroutine linresid(a, b, c, u, r, res, norm2, N, ierr)
  !-----------------------------------------------------------------
  ! Description:
  !    calculates the linear residual and its averaged 2-norm (WRMS)
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer, intent(in) :: N
  integer, intent(out) :: ierr
  double precision, dimension(N) :: a, b, c, u, r, res
  double precision :: norm2
  integer :: k

  !======= Internals ============

  ! initialze output to success
  ierr = 0

  ! compute linear residual at left of local subdomain
  res(1) = b(1)*u(1) + c(1)*u(2) - r(1)
  norm2 = res(1)**2

  ! compute linear residual in interior of subdomain
  do k=2,N-1
    res(k) = a(k)*u(k-1) + b(k)*u(k) + c(k)*u(k+1) - r(k)
    norm2 = norm2 + res(k)**2
  end do

  ! compute linear residual at right end of local subdomain
  res(N) = a(N)*u(N-1) + b(N)*u(N) - r(N)
  norm2 = norm2 + res(N)**2

  ! compute averaged 2-norm
  norm2 = sqrt(norm2)/N

  return

end subroutine linresid
!=================================================================
