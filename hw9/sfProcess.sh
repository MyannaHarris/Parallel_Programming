#!/bin/bash

#PBS -l nodes=4:ppn=1:physical
#PBS -l walltime=00:02:00
#PBS -o /home/Students/mharris5/lab6
#PBS -N lab6

date
PROGRAM="/home/Students/mharris5/lab6/para"
mpirun -np 4 -machinefile $PBS_NODEFILE $PROGRAM 400000
date
exit 0
