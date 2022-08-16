###################################################################
#  Makefile for lab on manager-worker paradigm
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

MPICC = mpicc
CFLAGS = -O0

chemistry_mpi.exe : chemistry_mpi.c chem_solver.c
	$(MPICC) $(CFLAGS) $^ -lm -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~

####### End of Makefile #######
