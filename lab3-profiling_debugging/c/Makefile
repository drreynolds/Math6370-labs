###################################################################
#  Makefile for Lab 3
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

all : driver1.exe driver2.exe

driver1.exe : driver1.c vectors.c Makefile
	gcc -O0 driver1.c vectors.c -o driver1.exe

driver2.exe : driver2.c tridiag_matvec.c Makefile
	gcc -O0 driver2.c tridiag_matvec.c -lm -o driver2.exe

####### End of Makefile #######
