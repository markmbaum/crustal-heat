#TEMPLATE CONFIG FILE

#c++ compiler
cxx=g++ # <--- GNU C++ compiler
#cxx=icpc # <--- Intel C++ compiler

#-------------------------------------------------------------------------------
#choose compilation flags

#compilation flags
flags= -Wall -Wextra -pedantic -O3 # <--- GNU compiler flags
#flags=-Wall -O3 # <--- Intel compiler flags

#compilation flag for openmp
omp=-fopenmp
#omp=-qopenmp

#-------------------------------------------------------------------------------
#set the path to libode top directory

odepath=path/to/libode
