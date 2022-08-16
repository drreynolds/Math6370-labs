###################################################################
#  Makefile for lab on manager-worker paradigm
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

MPIF90 = mpif90
FFLAGS = -O0

chemistry_mpi.exe : chemistry_mpi.f90 chem_solver.f90
	$(MPIF90) $(FFLAGS) $^ -lm -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~

####### End of Makefile #######
