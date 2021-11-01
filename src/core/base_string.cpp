#include <core/base_string.h>
#include <core/streamio.h>

using namespace falconOS::core::types;

namespace falconOS {
namespace core {
int len(char *str_) {
  int length;
  for (length = 0; str_[length] != '\0'; ++length)
    ;

  return length;
}

char *reverse(char *str1) {
  int i, n, temp;
  n = len(str1);

  for (i = 0; i < n / 2; i++) {
    temp = str1[i];
    str1[i] = str1[n - i - 1];
    str1[n - i - 1] = temp;
  }
  return str1;
}

char *to_string(const int int_) {
  int length;
  int temp = int_;
  for (length = 0; temp != 0; temp /= 10, ++length)
    ;

  char *str_;
  temp = int_;

  for (int i = 0; i < length; temp /= 10, ++i) {
    str_[i] = temp % 10 + '0';
  }
  str_[length] = '\0';

  return falconOS::core::reverse(str_);
}
} // namespace core
} // namespace falconOS