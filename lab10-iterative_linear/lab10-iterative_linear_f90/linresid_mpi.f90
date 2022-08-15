! -*- Mode: Fortran90; -*-
!-----------------------------------------------------------------
! Daniel R. Reynolds
! SMU, Mathematics
! Math 4370 / 6395
!=================================================================

subroutine linresid(a, b, c, u, r, res, norm2, local_N, comm, ierr)
  !-----------------------------------------------------------------
  ! Description:
  !    calculates the linear residual and its averaged 2-norm (WRMS)
  !-----------------------------------------------------------------
  !======= Inclusions ===========
  use mpi

  !======= Declarations =========
  implicit none

  integer, intent(in) :: local_N, comm
  integer, intent(out) :: ierr
  double precision, dimension(local_N) :: a, b, c, u, r, res
  double precision, intent(out) :: norm2
  double precision :: u_l, u_r, s_l, s_r, tmp
  integer :: my_id, nprocs, k
  integer :: status(MPI_STATUS_SIZE)

  !======= Internals ============

  ! Get MPI parallelism information from comm
  call MPI_Comm_size(comm, nprocs, ierr)
  if (ierr /= MPI_SUCCESS) then
     write(0,*) 'linresid error in MPI_Comm_size'
     return
  endif
  call MPI_Comm_rank(comm, my_id, ierr)
  if (ierr /= MPI_SUCCESS) then
     write(0,*) 'linresid error in MPI_Comm_rank'
     return
  endif

  ! initialize send/receive values
  s_l = u(1)
  s_r = u(local_N)
  u_l = 0.d0
  u_r = 0.d0

  ! phase 1: even procs exchange to right, odd ones exchange to left
  if (modulo(my_id,2) == 0) then

     if (my_id /= nprocs-1) then   ! check we're not last

        ! send to right w/ tag 100
        call MPI_Send(s_r, 1, MPI_REAL8, my_id+1, 100, comm, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Send'
           return
        endif

        ! recv from right w/ tag 101
        call MPI_Recv(u_r, 1, MPI_REAL8, my_id+1, 101, comm, status, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Recv'
           return
        endif

     endif

  else

     if (my_id /= 0) then          ! check we're not first

        ! recv from left w/ tag 100
        call MPI_Recv(u_l, 1, MPI_REAL8, my_id-1, 100, comm, status, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Recv'
           return
        endif

        ! send to left w/ tag 101
        call MPI_Send(s_l, 1, MPI_REAL8, my_id-1, 101, comm, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Send'
           return
        endif

     endif

  endif   ! if modulo(my_id,2)

  ! phase 2: even procs exchange to left, odd ones exchange to right
  if (modulo(my_id,2) == 1) then

     if (my_id /= nprocs-1) then   ! check we're not last

        ! send to right w/ tag 102
        call MPI_Send(s_r, 1, MPI_REAL8, my_id+1, 102, comm, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Send'
           return
        endif

        ! recv from right w/ tag 103
        call MPI_Recv(u_r, 1, MPI_REAL8, my_id+1, 103, comm, status, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Recv'
           return
        endif

     endif

  else

     if (my_id /= 0) then          ! check we're not first

        ! recv from left w/ tag 102
        call MPI_Recv(u_l, 1, MPI_REAL8, my_id-1, 102, comm, status, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Recv'
           return
        endif

        ! send to left w/ tag 103
        call MPI_Send(s_l, 1, MPI_REAL8, my_id-1, 103, comm, ierr)
        if (ierr /= MPI_SUCCESS) then
           write(0,*) 'linresid error in MPI_Send'
           return
        endif

     endif

  endif   ! if modulo(my_id,2)

  ! compute linear residual at left of local subdomain
  res(1) = a(1)*u_l + b(1)*u(1) + c(1)*u(2) - r(1)
  norm2 = res(1)**2

  ! compute linear residual in interior of subdomain
  do k=2,local_N-1
    res(k) = a(k)*u(k-1) + b(k)*u(k) + c(k)*u(k+1) - r(k)
    norm2 = norm2 + res(k)**2
  end do

  ! compute linear residual at right end of local subdomain
  k = local_N
  res(k) = a(k)*u(k-1) + b(k)*u(k) + c(k)*u_r - r(k)
  norm2 = norm2 + res(k)**2

  ! combine local 2-norms into global averaged 2-norm
  call MPI_Allreduce(norm2, tmp, 1, MPI_REAL8, MPI_SUM, comm, ierr)
  if (ierr /= 0) then
     write(0,*) 'linresid error: MPI_Allreduce failed'
     return
  end if
  norm2 = sqrt(tmp)/local_N/nprocs

  return

end subroutine linresid
!=================================================================
