###################################################################
#  Makefile for lab on iterative linear solvers with MPI
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

F90 = gfortran
MPIF90 = mpif90
FFLAGS = -O2

driver.exe: driver.f90 jacobi_solve.f90 linresid.f90 get_time.f90
	$(F90) $(FFLAGS) -lm -o $@ $^

driver_mpi.exe: driver_mpi.f90 jacobi_solve_mpi.f90 linresid_mpi.f90
	$(MPIF90) $(FFLAGS) -lm -o $@ $^

all : driver.exe driver_mpi.exe

clean :
	\rm *.exe *~

###################################################################
