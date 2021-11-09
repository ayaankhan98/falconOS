#pragma once

#include <core/streamio.h>

/// TODO: modify to exit when exit syscall is implemented

#define ASSERT(expression)                                                     \
  if ((expression) == 0) {                                                     \
    printf("ASSERT at LINE: ");                                                \
    printf(to_string(__LINE__));                                               \
    printf(" FILE: ");                                                         \
    printf(__FILE__);                                                          \
    printf("\n");                                                              \
  }
;