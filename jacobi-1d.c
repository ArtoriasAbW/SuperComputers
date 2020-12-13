/* Include benchmark-specific header. */
#include "jacobi-1d.h"
#include "omp.h"

/* Jacobi stencil computation 1d */

double bench_t_start, bench_t_end;

static double rtclock() {
  struct timeval Tp;
  int stat = gettimeofday(&Tp, NULL);
  if (stat != 0)
    printf("Error return from gettimeofday: %d", stat);
  return Tp.tv_sec + Tp.tv_usec * 1.0e-6;
}

void bench_timer_start() { 
  bench_t_start = rtclock(); 
  }

void bench_timer_stop() { 
  bench_t_end = rtclock(); 
}

void bench_timer_print() {
  printf("Time in seconds = %0.6lf\n", bench_t_end - bench_t_start);
}

static void init_array(int n, float *A, float *B) {
  int i;
#pragma omp parallel for 
  for (i = 0; i < n; i++) {
    A[i] = ((float)i + 2) / n;
    B[i] = ((float)i + 3) / n;
  }
}

static void print_array(int n, float *A) {

  fprintf(stderr, "==BEGIN DUMP_ARRAYS==\n");
  fprintf(stderr, "begin dump: %s", "A");
  for (int i = 0; i < n; i++) {
    if (i % 20 == 0)
      fprintf(stderr, "\n");
    fprintf(stderr, "%0.2f ", A[i]);
  }
  fprintf(stderr, "\nend   dump: %s\n", "A");
  fprintf(stderr, "==END   DUMP_ARRAYS==\n");
}

static void kernel_jacobi_1d(int tsteps, int n, float *A, float *B) {
  int t, i;
  for (t = 0; t < tsteps; ++t) {
#pragma omp parallel for shared(A, B) schedule(static) private(i)
    for (i = 1; i < n - 1; ++i) {
      B[i] = (A[i - 1] + A[i] + A[i + 1]) / 3.0;
    }
#pragma omp parallel for shared(A, B) schedule(static) private(i)
    for (i = 1; i < n - 1; ++i) {
      A[i] = (B[i - 1] + B[i] + B[i + 1]) / 3.0;
    }
  }
}

int main(int argc, char **argv) {
  int n = N;
  int tsteps = TSTEPS;
  float *A = (float*)malloc(n * sizeof(float)); 
  float *B = (float*)malloc(n * sizeof(float)); 

  bench_timer_start();

  init_array(n, A, B);

  kernel_jacobi_1d(tsteps, n, A, B); 


  bench_timer_stop(); 
  bench_timer_print(); 

#ifdef PRINT
    print_array(n, A);
#endif

  free(A);
  free(B);

  return 0;
}
