! -*- Mode: Fortran90; -*-
!=================================================================

program Bug6
  !-----------------------------------------------------------------
  ! FILE: omp_bug6.f90
  ! DESCRIPTION:
  !   This program fails due to incorrectly scoped variables.  
  !   Compare to omp_orphan.c.
  ! AUTHOR: Blaise Barney  6/05
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: i
  real*8 :: sum
  integer, parameter :: N = 100
  real*8 :: A(N), B(N)

  !======= Internals ============

  do i=1, N
     A(i) = 1.d0 * i
     B(i) = A(i)
  enddo
  sum = 0.d0

  !$omp parallel shared(sum)
  call DotProd(A, B)
  !$omp end parallel

  write(*,*) "Sum = ", sum

end program Bug6
!=================================================================



subroutine DotProd(A, B)
  !-----------------------------------------------------------------
  ! DOTPROD: computes the dot-product of the two vectors A and B.
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: i, TID
  integer, external :: omp_get_thread_num
  integer, parameter :: N = 100
  real*8 :: A(N), B(N), sum

  !======= Internals ============

  TID = omp_get_thread_num()
  !$omp do reduction(+:sum)
  do i=1, N
     sum = sum + (A(i)*B(i))
     print *, '  TID= ',TID,'i= ',i
  enddo

  return

end subroutine DotProd
!=================================================================
