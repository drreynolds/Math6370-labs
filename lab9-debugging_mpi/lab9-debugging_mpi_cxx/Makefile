###################################################################
#  Makefile for lab on debugging MPI programs
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

MPICXX = mpicxx
CXXFLAGS = -O2

quadrature.exe : quadrature.cpp
	$(MPICXX) $(CXXFLAGS) $^ -lm -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~

####### End of Makefile #######
