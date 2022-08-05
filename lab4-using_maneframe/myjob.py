#!/usr/bin/env python3
# Python script that approximates pi using a 2-point
#   trapezoidal quadrature rule of n subintervals, where
#   n is provided as a command-line argument.
# Daniel R. Reynolds
# SMU Mathematics

# imports
import numpy as np
import time
import sys

# start the timer
stime = time.time()

# access the command line argument
nargs = len(sys.argv)
if (nargs < 2):
   sys.exit("Error: integer command line argument required")
n = int(sys.argv[1])

# compute the mesh spacing and shortcut
h  = 1.0/n
h2 = 0.5*h

# set the quadrature nodes
x1 = -1.0
x2 =  1.0

# initialize the result
pi = 0.0

# loop over subintervals, computing approximation
for i in range(n):

   # compute evaluation points on this interval
   xmid = i*h + h2;
   n1 = xmid + h2*x1
   n2 = xmid + h2*x2

   # evaulate integrand at nodes
   f1 = 4.0 / (1.0 + n1**2)
   f2 = 4.0 / (1.0 + n2**2)

   # update approximation
   pi += h2*(f1 + f2)


# compute relative error
err = (np.pi - pi)/np.pi

# stop the timer
ftime = time.time()

# output results
print("Using %i subintervals:" % n)
print("  pi approximation is: %.16f" % pi)
print("  with relative error: %g" % err)
print("  total time required: %g" % (ftime-stime) )
