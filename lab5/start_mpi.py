import os

executable = "./lab5.out"
processor_threads = 12
os.system("mpicc -o lab5.out lab5_parallel_mpi.c")

for i in range(1, 25):
    os.system(f"mpiexec -n {i} --oversubscribe ./lab5.out")
