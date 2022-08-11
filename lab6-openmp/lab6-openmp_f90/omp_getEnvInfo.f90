! -*- Mode: Fortran90; -*-
!=================================================================

program GetInfo
  !-----------------------------------------------------------------
  ! FILE: omp_getEnvInfo.f90
  ! DESCRIPTION:
  !   OpenMP Example - Get Environment Information
  !   The master thread queries and prints selected environment
  !   information.
  ! AUTHOR: Blaise Barney  7/06
  ! Upgraded to F90, free-source form: Daniel R. Reynolds, 2/2009
  !-----------------------------------------------------------------
  !======= Inclusions ===========

  !======= Declarations =========
  implicit none

  integer :: NThreads, TID, Procs, MaxT
  integer, external :: omp_get_num_threads, omp_get_thread_num, &
       omp_get_num_procs, omp_get_max_threads, &
       omp_in_parallel, omp_get_dynamic, omp_get_nested
  logical :: InPar, Dyn, Nested

  !======= Internals ============

  ! Start parallel region
  !$omp parallel private(TID)
  
  ! Obtain thread number
  TID = omp_get_thread_num()
  
  ! Only master thread does this
  if (TID == 0) then
     
     print *, 'Thread',tid,'getting environment information'
     
     ! Get environment information
     Procs    = omp_get_num_procs() 
     NThreads = omp_get_num_threads()
     MaxT     = omp_get_max_threads()
     InPar    = omp_in_parallel()
     Dyn      = omp_get_dynamic()
     Nested   = omp_get_nested()

     ! Print environment information
     print *, 'Number of processors = ', Procs
     print *, 'Number of threads = ', NThreads
     print *, 'Max threads = ', MaxT
     print *, 'In parallel? = ', InPar
     print *, 'Dynamic threads enabled? = ', Dyn
     print *, 'Nested parallelism supported? = ', Nested

  end if
  
  ! Done
  !$omp end parallel

end program GetInfo
!=================================================================

