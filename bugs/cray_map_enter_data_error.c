#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#define ISIZ1  12
#define ISIZ2  12
#define ISIZ3  12


double u[5][ISIZ2*2];

void omp_device_mem_init() {
  #pragma omp target enter data \
          map (to: u[0:5][0:(ISIZ2*2)])
}

void omp_device_mem_delete() {
  #pragma omp target exit data \
          map (delete: u[0:5][0:(ISIZ2*2)])
} 

int main () {

  return 0;
}
