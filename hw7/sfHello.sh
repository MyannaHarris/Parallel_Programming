#!/bin/bash

#PBS -l nodes=4:ppn=2:physical
#PBS -l walltime=00:10:00
#PBS -o /home/Students/mharris5/lab4
#PBS -N lab4

date
PROGRAM="/home/Students/mharris5/lab4/mpihello"
mpirun -machinefile $PBS_NODEFILE $PROGRAM
date
exit 0
