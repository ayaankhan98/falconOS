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

char *to_string(const int val, int base) {
  int length;
  int temp = val;
  for (length = 0; temp != 0; temp /= base, ++length)
    ;

  char *str_;
  temp = val;

  for (int i = 0; i < length; temp /= base, ++i) {
    str_[i] = temp % base + '0';
    if(str_[i] > '9') {
      str_[i] -= '9' + 1;
      str_[i] += 'A';
    }
  }
  switch(base) {
    case 16:
      str_[length] = 'x';
      str_[length + 1] = '0';
      str_[length + 2] = '\0';
      break;
    case 2:
      str_[length] = 'b';
      str_[length + 1] = '0';
      str_[length + 2] = '\0';
      break;
    case 10:
    default:
      str_[length] = '\0';
  }

  return strrev(str_);
}
} // namespace core
} // namespace falconOS