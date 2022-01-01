#include <libc/ctype.h>

int isalnum(int c) {
  return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isalpha(int c) {
  return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int islower(int c) {
  return (c >= 'a' && c <= 'z');
}

int isupper(int c) {
  return (c >= 'A' && c <= 'Z');
}

int isdigit(int c) {
  return (c >= '0' && c <= '9');
}

int isxdigit(int c) {
  return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

int isspace(int c) {
  return (c == ' ' || c == '\t');
}
