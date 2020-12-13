
all: linear parallel

parallel:
	gcc -std=c99 -O2 -DMEGALARGE_DATASET -fopenmp -o jacobi-1d_par jacobi-1d.c

linear:
	gcc -std=c99 -O2 -DMEGALARGE_DATASET -o jacobi-1d jacobi-1d.c

clean:
	rm jacobi-1d_par jacobi-1d
