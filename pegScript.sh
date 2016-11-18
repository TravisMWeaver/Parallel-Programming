#!/bin/bash
#BSUB -J Project1Job
#BSUB -o proj1.out
#BSUB -q general
#BSUB -a openmpi
#BSUB -n 2
#

module load intel
rm proj1.out
icc -openmp matVectorProd.cpp -o matVectorProd
./matVectorProd