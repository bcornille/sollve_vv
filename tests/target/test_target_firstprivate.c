// RUN: %libomptarget-compile-run-and-check-aarch64-unknown-linux-gnu
// RUN: %libomptarget-compile-run-and-check-powerpc64-ibm-linux-gnu
// RUN: %libomptarget-compile-run-and-check-powerpc64le-ibm-linux-gnu
// RUN: %libomptarget-compile-run-and-check-x86_64-pc-linux-gnu

//===--test_target_firstprivate.c ------------------------------------------------===//
// 
// OpenMP API Version 4.5 Nov 2015
//
//Testing first private clause with target directive
////===----------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>

#define N 10

int main() {
  int compute_array[4][N];
  int errors = 0;
  int i,j, isHost = -1;//, p_val=-1;
  
 
  for (i=0; i<4; i++) 
    for (j=0; j<N; j++) 
      compute_array[i][j] = 0;

  omp_set_num_threads(4);
#pragma omp parallel
{
  int p_val = omp_get_thread_num();

#pragma omp target map(tofrom:compute_array) map(tofrom: isHost) firstprivate(p_val)
  {
    /*Record where the computation was executed*/
    isHost = omp_is_initial_device();
    for (i = 0; i < N; i++)
      compute_array[p_val][i] = 100;
  } // End target
}//end-parallel

  for (i=0; i<4; i++){ 
    for (j=0; j<N; j++){
      if(compute_array[i][j] != 100){
        errors += 1;
        printf("compute_array[%d][%d] = %d\n",i,j,compute_array[i][j]);
      }
    }
  }//end-for
  if (errors) {
    printf("Test failed on %s\n",isHost ? "host":"device");
  }
  else {
    printf("Test passed on %s\n", isHost ? "host":"device");
  }
  return errors;

}
