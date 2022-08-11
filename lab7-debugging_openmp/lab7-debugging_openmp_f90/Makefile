###################################################################
#  Makefile for Lab 7
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

F90 = gfortran
FFLAGS = -O2
OMPFLAG = -fopenmp

quadrature.exe : quadrature.f90 get_time.f90
	$(F90) $(FFLAGS) $(OMPFLAG) $^ -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~

####### End of Makefile #######
