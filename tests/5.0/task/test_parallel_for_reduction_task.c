//===--- test_parallel_for_reduction_task.c ---------------------------------===//
//
// OpenMP API Version 5.0 Nov 2018
//
// This test checks the parallel for directive with the reduction clause and
// task modifier. It performs simple array operations which are added to
// reduction variables in two tasks, one with the in_reduction clause and the
// other without. Only the in_reduction task should participate in the
// reduction.
//
////===----------------------------------------------------------------------===//
#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define N 1024

int test_parallel_for_reduction_task() {
  OMPVV_INFOMSG("test_parallel_for_reduction_task");
  int errors = 0;
  int num_threads = -1;
  int y[N];
  int z[N];
  int sum = 0;
  int expected_sum = 0;

  for (int i = 0; i < N; i++) {
    y[i] = i + 1;
    z[i] = 2*(i + 1);
  }

#pragma omp parallel for reduction(task, +: sum) num_threads(OMPVV_NUM_THREADS_HOST) shared(y, z, num_threads)
  for (int i = 0; i < N; i++) {
#pragma omp task in_reduction(+: sum)
    sum += y[i]*z[i];
#pragma omp task
    sum += y[i]*z[i];        // These writes should not be included in the reduction
    if (omp_get_thread_num() == 0) {
      num_threads = omp_get_num_threads();
    }
  }

  for (int i = 0; i < N; i++) {
    expected_sum += y[i]*z[i];
  }

  OMPVV_TEST_AND_SET_VERBOSE(errors, sum != expected_sum);

  OMPVV_WARNING_IF(num_threads == 1, "Test ran with one thread, so parallelism of taskloop can't be guaranteed.");
  OMPVV_ERROR_IF(num_threads < 1, "Test returned an invalid number of threads.");
  OMPVV_TEST_AND_SET_VERBOSE(errors, num_threads < 1);

  return errors;
}


int main() {
  int errors = 0;

  OMPVV_TEST_AND_SET_VERBOSE(errors, test_parallel_for_reduction_task());

  OMPVV_REPORT_AND_RETURN(errors);
}
