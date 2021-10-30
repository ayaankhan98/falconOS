#include <core/base_string.h>

using namespace fineOS::core::types;

namespace fineOS {
namespace core {
uint8_t len(char *str_) {
  uint8_t length;
  for (length = 0; str_[length] != '\0'; ++length)
    ;

  return length;
}

char *reverse(char *str_) {
  uint8_t n = fineOS::core::len(str_);
  char *temp;
  for (uint8_t i = 0; i < n; ++i) {
    temp[i] = str_[n - i - 1];
  }
  temp[n] = '\0';
  str_ = temp;
  return str_;
}

char *to_string(const uint8_t int_) {
  uint8_t length;
  uint8_t temp = int_;
  for (length = 0; temp != 0; temp /= 10, ++length)
    ;

  char *str_;
  temp = int_;

  for (int i = 0; i < length; temp /= 10, ++i) {
    str_[i] = temp % 10 + '0';
  }
  str_[length] = '\0';

  return fineOS::core::reverse(str_);
}
} // namespace core
} // namespace fineOS