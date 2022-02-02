//===--- test_strict_grainsize.c -----------------------------------------------===//
//
//  OpenMP API Version 5.1 Aug 2021
//
// This test checks the behavior of taskloop's clause grainsize, when the strict 
// modifier is present. The grainsize strict expressions specifies the exact number of 
// logical iterations which should be performed per task, except for the sequentially
// last iteration which may have fewer than the specified iterations. 
////===--------------------------------------------------------------------------------------===//
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"
#include <math.h>

#define N 1024

int errors;

int test_strict_grainsize() {
  int arr[N];
  int sum = 0;
  int parallel_sum = 0; 
  for (int i=0; i<N; i++){
        arr[i] = i;
        sum += arr[i];
 }
#pragma omp parallel shared(arr, parallel_sum)
  {  
#pragma single 
	  { 
#pragma omp taskloop grainsize(1000)
  for (int i = 0; i < N; i++) {
  	parallel_sum += arr[i];
  }
  }
  }
  OMPVV_TEST_AND_SET(errors, parallel_sum == sum);
  OMPVV_INFOMSG_IF(sum == 0, "Array was not initialzed.");
  OMPVV_INFOMSG_IF(parallel_sum == sum, "Test passed.");
  return errors;
}

int main() {
  errors = 0;
  OMPVV_TEST_OFFLOADING;
  OMPVV_TEST_AND_SET_VERBOSE(errors, test_strict_grainsize() != 0);
  OMPVV_REPORT_AND_RETURN(errors);
}
