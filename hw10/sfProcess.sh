#!/bin/bash

#PBS -l nodes=4:ppn=1:physical
#PBS -l walltime=00:02:00
#PBS -o /home/Students/mharris5/lab5
#PBS -N lab5

date
PROGRAM="/home/Students/mharris5/lab5/mpiProcess"
mpirun -np 4 -machinefile $PBS_NODEFILE $PROGRAM 16
date
exit 0
