###################################################################
#  Makefile for lab on iterative linear solvers with MPI
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

CXX = g++
MPICXX = mpicxx
CXXFLAGS = -O2

driver.exe: driver.cpp jacobi_solve.cpp linresid.cpp
	$(CXX) $(CXXFLAGS) -lm -o $@ $^

driver_mpi.exe: driver_mpi.cpp jacobi_solve_mpi.cpp linresid_mpi.cpp
	$(MPICXX) $(CXXFLAGS) -lm -o $@ $^

all : driver.exe driver_mpi.exe

clean :
	\rm *.exe *~

###################################################################
