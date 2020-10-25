#!/bin/bash
#SBATCH -p GPU
#SBATCH -N 4
#SBATCH -t 00:20:00
#SBATCH --ntasks-per-node 1
#SBATCH --exclusive
#SBATCH --gres=gpu:p100:2

export OMP_NUM_THREADS=1

echo "Bridges P100 GPU (mpicc -acc)."
echo ""
echo "1 MPI Node:"
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 1 /home/rs170530/Pgm5/Pgm5.p100

echo ""
echo "2 MPI Nodes:"
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 2 /home/rs170530/Pgm5/Pgm5.p100

echo ""
echo "3 MPI Nodes:"
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 3 /home/rs170530/Pgm5/Pgm5.p100

echo ""
echo "4 MPI Nodes:"
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100
mpirun -np 4 /home/rs170530/Pgm5/Pgm5.p100