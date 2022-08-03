###################################################################
#  Makefile for Lab 2
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 4370 / 6370
###################################################################

driver.exe : one_norm.cpp vector_product.cpp vector_sum.cpp vector_difference.cpp driver.cpp
	g++ -c one_norm.cpp
	g++ -c vector_product.cpp
	g++ -c vector_sum.cpp
	g++ -c vector_difference.cpp
	g++ -c driver.cpp
	g++ -O0 -o driver.exe one_norm.o vector_product.o vector_sum.o vector_difference.o driver.o -lm

clean :
	rm -f *.o

####### End of Makefile #######
