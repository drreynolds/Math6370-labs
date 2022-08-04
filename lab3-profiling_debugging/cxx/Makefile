###################################################################
#  Makefile for Lab 3
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

all : driver1.exe driver2.exe

driver1.exe : driver1.cpp vectors.cpp Makefile
	g++ -O0 driver1.cpp vectors.cpp -o driver1.exe

driver2.exe : driver2.cpp tridiag_matvec.cpp Makefile
	g++ -O0 driver2.cpp tridiag_matvec.cpp -o driver2.exe

####### End of Makefile #######
