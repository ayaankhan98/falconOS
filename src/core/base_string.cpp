#include <core/base_string.h>
#include <core/streamio.h>

using namespace falconOS::core::types;

namespace falconOS {
namespace core {
int strlen(char *str_) {
  int length;
  for (length = 0; str_[length] != '\0'; ++length)
    ;
  return length;
}

char *strrev(char *str) {
  int i, n, temp;
  n = strlen(str);

  for (i = 0; i < n / 2; i++) {
    temp = str[i];
    str[i] = str[n - i - 1];
    str[n - i - 1] = temp;
  }
  return str;
}

char *to_string(const int val) {
  int length;
  int temp = val;
  for (length = 0; temp != 0; temp /= 10, ++length)
    ;

  char *str_;
  temp = val;

  for (int i = 0; i < length; temp /= 10, ++i) {
    str_[i] = temp % 10 + '0';
  }
  str_[length] = '\0';

  return strrev(str_);
}
} // namespace core
} // namespace falconOS