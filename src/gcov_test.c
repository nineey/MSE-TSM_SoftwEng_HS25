/*
 * Copyright (c) 2023-2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gcov_test.h"
#include <stdio.h>
// #pragma GCC optimize("O3,inline")

static int Calc(int i, int j) {
  int res;
  if (i==0) {
    res = i+1;
  } else if (i>0 && j<10) {
    res = i+j;
  }
  return res;
}

static int Value(int i) {
  if (i==3) {
    return 5;
  }
  return 1;
}

static void Test2(int *p) {
  if (*p!=0) {
    if (Value(*p)==5) {
      printf("value is 5\n");
      *p = 0;
    }
  }
}

void gcov_test(int i) {
  int j = Calc(i, i*2);
  Test2(&j);
  if (j==0) {
    printf("j is zero!\n");
  } else {
    printf("j is not zero!\n");
  }
  /* Additional calls for 100% coverage */
  Calc(0, 5);     
  Calc(3, 2);      
  Calc(1, 1);     
  Value(3);        
  Value(1);        
  int x = 3;
  Test2(&x);       
  int y = 1;
  Test2(&y);       
  int z = 0;
  Test2(&z);       

  // Test case for line 41
  int a = Calc(1, 2);  // a = 3
  Test2(&a);           // Value(3)==5, so returns 0
  if (a==0) {
    printf("j is zero!\n");
  }
}



