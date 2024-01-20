import os

executable = "./lab6.out"
os.system(f"mpicc -o {executable} lab6_parallel_mpi.c")

for i in range(1, 25):
    os.system(f"mpiexec -n {i} --oversubscribe {executable}")
