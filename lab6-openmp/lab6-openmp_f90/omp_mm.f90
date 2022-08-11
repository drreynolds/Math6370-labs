! -*- Mode: Fortran90; -*-
!=================================================================

program MatMult
  !-----------------------------------------------------------------
  ! FILE: omp_mm.f90
  ! DESCRIPTION:  
  !   OpenMp Example - Matrix Multiply
  !   Demonstrates a matrix multiply using OpenMP. Threads share 
  !   row iterations according to a predefined chunk size.
  ! AUTHOR: Blaise Barney, 1/5/04
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: TID, NThreads, I, J, K, chunk
  integer, external :: omp_get_num_threads, omp_get_thread_num
  integer, parameter :: NRA = 62     ! number of rows in matrix A 
  integer, parameter :: NCA = 15     ! number of columns in matrix A
  integer, parameter :: NCB = 7      ! number of columns in matrix B
  real*8 :: A(NRA,NCA), B(NCA,NCB), C(NRA,NCB)

  !======= Internals ============

  ! Set loop iteration chunk size 
  chunk = 10

  ! Spawn a parallel region explicitly scoping all variables
  !$omp parallel shared(A,B,C,NThreads,chunk) private(TID,I,J,K)
  TID = omp_get_thread_num()
  if (TID == 0) then
     NThreads = omp_get_num_threads()
     print *, 'Starting matrix multiple example with',NThreads,'threads'
     print *, 'Initializing matrices'
  end if

  ! Initialize matrices
  !$omp do schedule(static, chunk)
  do I=1, NRA
     do J=1, NCA
        A(I,J) = (I-1)+(J-1)
     enddo
  enddo
  !$omp do schedule(static, chunk)
  do I=1, NCA
     do J=1, NCB
        B(I,J) = (I-1)*(J-1)
     enddo
  enddo
  !$omp do schedule(static, chunk)
  do I=1, NRA
     do J=1, NCB
        C(I,J) = 0
     enddo
  enddo

  ! Do matrix multiply sharing iterations on outer loop
  ! Display who does which iterations for demonstration purposes
  print *, 'Thread', TID, 'starting matrix multiply...'
  !$omp do schedule(static, chunk)
  do I=1, NRA
     print *, 'Thread', TID, 'did row', I
     do J=1, NCB
        do K=1, NCA
           C(I,J) = C(I,J) + A(I,K) * B(K,J)
        enddo
     enddo
  enddo
  ! End of parallel region 
  !$omp end parallel

  ! Print results
  print *, '******************************************************'
  print *, 'Result Matrix:'
  do I=1, NRA
     do J=1, NCB
        write(*,'(2x,f8.2)') C(I,J)
     enddo
     print *, ' '
  enddo
  print *, '******************************************************'
  print *, 'Done.'
  
end program MatMult
!=================================================================
