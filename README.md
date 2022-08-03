# Math6370-labs

Codes for student "labs" for the course: Parallel Scientific Computing (MATH 4370/6370) at [Southern Methodist University](https://www.smu.edu).

These codes require working C, C++11, and Fortran-90 compilers that support OpenMP, a working MPI installation (preferably with the `mpicc`, `mpicxx` and `mpifort` wrapper scripts and `mpiexec` launcher), and the "make" program, and are designed to be compiled/run at the Linux/OSX command line.  The optional RAJA lab additionally requires that [RAJA](https://github.com/LLNL/RAJA) v0.12.1 or higher is installed, with the [CUDA](https://developer.nvidia.com/cuda-downloads) backend (this introduces additional dependencies) -- if your computer has an NVIDIA graphics card or GPU accelerator, then RAJA and its dependencies maybe easily installed using [Spack](https://github.com/spack/spack).

[Daniel R. Reynolds](https://people.smu.edu/dreynolds)  
[Mathematics @ SMU](https://www.smu.edu/math)
