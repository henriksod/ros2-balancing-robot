// Copyright (c) 2023, Henrik Söderlund

#include <ztest.h>

/* Make private functions public for access.
   I think this is not a clean way to do it, but for my purposes it was just fine. */
#define private public

#include "../src/Classyouwanttest.h"

void test_function(void) {
  Classyouwanttest classyouwanttest;

  /* your zassert instructions here: */
  zassert_equal(classyouwanttest.funcXY(...), ..., ...);
}

void test_main(void) {
  ztest_test_suite(common,
                   ztest_unit_test(test_function));
  ztest_run_test_suite(common);
}
