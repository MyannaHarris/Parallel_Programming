#!/bin/bash

#PBS -l nodes=4:ppn=1:physical
#PBS -l walltime=00:02:00
#PBS -o /home/Students/mharris5/proj
#PBS -N proj

date
PROGRAM="/home/Students/mharris5/proj/mpiProcess"
mpirun -np 4 -machinefile $PBS_NODEFILE $PROGRAM 16
date
exit 0
