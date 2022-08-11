! -*- Mode: Fortran90; -*-
!=================================================================

program Orphan
  !-----------------------------------------------------------------
  ! FILE: omp_orphan.f90
  ! DESCRIPTION:
  !   OpenMP Example - Parallel region with an orphaned directive
  !   This example demonstrates a dot product being performed by 
  !   an orphaned loop reduction construct.  Scoping of the 
  !   reduction variable is critical.
  ! AUTHOR: Blaise Barney  5/99
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: i
  integer, parameter :: N = 100
  real*8 :: A(N), B(N), sum

  !======= Internals ============

  do i=1, N
     A(i) = 1.d0 * i
     B(i) = A(i)
  enddo
  sum = 0.d0

  !$omp parallel
  call DotProd(A, B, sum)
  !$omp end parallel

  write(*,*) "Sum = ", sum

end program Orphan
!=================================================================



subroutine DotProd(A, B, sum)
  !-----------------------------------------------------------------
  ! DOTPROD: Computes the dot-product and places the result in SUM.
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
     print *, '  TID= ',TID,'I= ',i
  enddo
  !$omp end do

  return

end subroutine DotProd
!=================================================================
