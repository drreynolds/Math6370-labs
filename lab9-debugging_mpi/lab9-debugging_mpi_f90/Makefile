###################################################################
#  Makefile for lab on debugging MPI programs
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

MPIF90 = mpif90
FFLAGS = -O2

quadrature.exe : quadrature.f90
	$(MPIF90) $(FFLAGS) $^ -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~

####### End of Makefile #######
