###################################################################
#  Makefile for lab on manager-worker paradigm
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

MPICXX = mpicxx
CXXFLAGS = -O0

chemistry_mpi.exe : chemistry_mpi.cpp chem_solver.cpp
	$(MPICXX) $(CXXFLAGS) $^ -lm -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~

####### End of Makefile #######
