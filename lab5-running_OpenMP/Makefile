###################################################################
#  Makefile for Lab 5
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

CXX = g++
CFLAGS = -O2
OMP = -fopenmp

driver.exe : driver.cpp vectors.cpp Makefile
	$(CXX) $(OMP) $(CFLAGS) driver.cpp vectors.cpp -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~


####### End of Makefile #######
