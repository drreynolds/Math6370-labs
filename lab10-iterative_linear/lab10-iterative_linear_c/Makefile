###################################################################
#  Makefile for lab on iterative linear solvers with MPI
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

CC = gcc
MPICC = mpicc
CFLAGS = -O2

driver.exe: driver.c jacobi_solve.c linresid.c
	$(CC) $(CFLAGS) -lm -o $@ $^

driver_mpi.exe: driver_mpi.c jacobi_solve_mpi.c linresid_mpi.c
	$(MPICC) $(CFLAGS) -lm -o $@ $^

all : driver.exe driver_mpi.exe

clean :
	\rm *.exe *~

###################################################################
