! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU Mathematics
! Math 4370 / 6370
!=================================================================


subroutine vector_sum(l,m,n,x,y,z)
  !-----------------------------------------------------------------
  ! Description:
  !    Computes the linear combination z = x + y
  !
  ! Arguments:
  !    l - integer (input), length of vectors
  !    m - integer (input), length of vectors
  !    n - integer (input), length of vectors
  !    x - double (input), vector
  !    y - double (input), vector
  !    z - double (output), vector
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none
  integer, intent(in) :: l, m, n
  double precision, intent(in) :: x(l,m,n), y(l,m,n)
  double precision, intent(out) :: z(l,m,n)

  integer :: i, j, k

  !======= Internals ============

  ! loop over the dimensions to compute the vector sum
  do i=1,l,1
     do j=1,m,1
        do k=1,n,1
           z(i,j,k) = x(i,j,k) + y(i,j,k)
        enddo
     enddo
  enddo

  return

end subroutine vector_sum
!=================================================================



subroutine vector_scale(l,m,n,a,x,z)
  !-----------------------------------------------------------------
  ! Description:
  !    Computes the scaled vector z = a*x
  !
  ! Arguments:
  !    l - integer (input), length of vectors
  !    m - integer (input), length of vectors
  !    n - integer (input), length of vectors
  !    a - double (input), scalar
  !    x - double (input), vector
  !    z - double (output), vector
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none
  integer, intent(in) :: l, m, n
  double precision, intent(in) :: a, x(l,m,n)
  double precision, intent(out) :: z(l,m,n)

  integer :: i, j, k

  !======= Internals ============

  ! loop over the dimensions to compute the scaled vector
  do k=1,n,1
     do j=1,m,1
        do i=1,l,1
           z(i,j,k) = a*x(i,j,k)
        enddo
     enddo
  enddo

  return

end subroutine vector_scale
!=================================================================



subroutine vector_infnorm(l,m,n,x,norm)
  !-----------------------------------------------------------------
  ! Description:
  !    Computes the infinity-norm, norm = ||x||_{\infty}
  !
  ! Arguments:
  !    l - integer (input), length of vector
  !    m - integer (input), length of vector
  !    n - integer (input), length of vector
  !    x - double (input), vector
  !    norm - double (output), norm of vector
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none
  integer, intent(in) :: l, m, n
  double precision, intent(in) :: x(l,m,n)
  double precision, intent(out) :: norm

  integer :: i, j, k

  !======= Internals ============

  ! loop over the dimensions to compute the infinity norm
  norm = 0.d0
  do k=1,n,1
     do j=1,m,1
        do i=1,l,1
           if (abs(x(i,j,k)) > norm)  norm = abs(x(i,j,k))
        enddo
     enddo
  enddo

  return

end subroutine vector_infnorm
!=================================================================



subroutine vector_dotprod(l,m,n,x,y,prod)
  !-----------------------------------------------------------------
  ! Description:
  !    Computes the dot-product, prod = x \dot y
  !
  ! Arguments:
  !    l - integer (input), length of vectors
  !    m - integer (input), length of vectors
  !    n - integer (input), length of vectors
  !    x - double (input), vector
  !    y - double (input), vector
  !    prod - double (output), dot-product of vectors
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none
  integer, intent(in) :: l, m, n
  double precision, intent(in) :: x(l,m,n), y(l,m,n)
  double precision, intent(out) :: prod

  integer :: i, j, k

  !======= Internals ============

  ! loop over the dimensions to compute the dot-product
  prod = 0.d0
  do k=1,n,1
     do j=1,m,1
        do i=1,l,1
           prod = prod + x(i,j,k)*y(i,j,k)
        enddo
     enddo
  enddo

  return

end subroutine vector_dotprod
!=================================================================
