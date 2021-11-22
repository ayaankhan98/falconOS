#pragma once

#include <core/types.h>

namespace falconOS {
namespace core {
int strlen(char *str_);

char *strrev(char *str_);

char *to_string(const int val, int base = 10);
} // namespace core
} // namespace falconOS
