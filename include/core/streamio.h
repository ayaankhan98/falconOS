#pragma once

#include <core/base_string.h>
#include <core/types.h>

namespace falconOS {
namespace core {
#define VIDEO_MEMORY_ADDRESS 0xB8000

#define LOG(str) log(/* __LINE__, */ __FUNCTION__, __FILE__, str)

enum screen { ROWS = 25, COLUMNS = 80 };

enum color {
  BLACK_ON_BLACK = 0x0000,
  BLUE_ON_BLACK = 0x0100,
  GREEN_ON_BLACK = 0x0200,
  CYAN_ON_BLACK = 0x0300,
  RED_ON_BLACK = 0x0400,
  PURPLE_ON_BLACK = 0x0500,
  BROWN_ON_BLACK = 0x0600,
  GRAY_ON_BLACK = 0x0700,
  DARK_GRAY_ON_BLACK = 0x0800,
  LIGHT_BLUE_ON_BLACK = 0x0900,
  LIGHT_GREEN_ON_BLACK = 0x0A00,
  LIGHT_CYAN_ON_BLACK = 0x0B00,
  LIGHT_RED_ON_BLACK = 0x0C00,
  LIGHT_PURPLE_ON_BLACK = 0x0D00,
  YELLOW_ON_BLACK = 0x0E00,
  WHITE_ON_BLACK = 0x0F00
};

enum logLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  FATAL = 3,
  ERROR = 4,
  TRACE = 5
};

void printf(const char *str_, const color color_ = color::GRAY_ON_BLACK,
            const bool blink = false);

void printfHexa(types::uint8_t hexaDigit_);

void log(/*int line_, */ const char *function_, const char *file_,
         const char *message_);
void log(const char *message_, const logLevel level_ = logLevel::INFO);
} // namespace core
} // namespace falconOS
