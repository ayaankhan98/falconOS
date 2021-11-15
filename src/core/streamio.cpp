#include <core/streamio.h>

using namespace falconOS::core::types;

namespace falconOS {
namespace core {
void printf(const char *str_, const color color_, const bool blink) {
  static uint16_t *videoMemory = (uint16_t *)VIDEO_MEMORY_ADDRESS;

  static uint8_t cursorX = 0, cursorY = 0;

  for (int32_t i = 0; str_[i] != '\0'; ++i) {
    switch (str_[i]) {
    case '\n':
      cursorY++;
      cursorX = 0;
      break;

    default:
      videoMemory[screen::COLUMNS * cursorY + cursorX] =
          (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0x00FF) | (color_ | (blink ? 0x8000 : 0x0000));
      videoMemory[screen::COLUMNS * cursorY + cursorX] =
          (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0xFF00) | str_[i];
      cursorX++;
      break;
    }

    if (cursorX >= screen::COLUMNS) {
      cursorY++;
      cursorX = 0;
    }

    /// if cursor Y goes beyond the number of row, it means screen is
    /// filled and we have to scroll the screen by one line
    if (cursorY >= screen::ROWS) {
      for (int i = screen::COLUMNS; i < (screen::ROWS - 1) * screen::COLUMNS;
           i++) {
        videoMemory[i] = videoMemory[i + screen::COLUMNS];
      }
      for (i = (screen::ROWS - 1) * screen::COLUMNS; i < 25 * screen::COLUMNS;
           i++) {
        videoMemory[i] = ' ';
      }
      cursorY = (screen::ROWS - 1);

      /** previous version: where we just clears the whole screen once it get
       * filled
       */
      // for (cursorY = 0; cursorY <= screen::ROWS; cursorY++) {
      //   for (cursorX = 0; cursorX <= screen::COLUMNS; cursorX++) {
      //     videoMemory[screen::COLUMNS * cursorY + cursorX] =
      //         (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0xFF00) | '
      //         ';
      //   }
      // }
      // cursorX = 0, cursorY = 0;
    }
  }
}

void printfHexa(types::uint8_t hexaDigit_) {
  char *foo = "00";
  char *hex = "0123456789ABCDEF";
  foo[0] = hex[(hexaDigit_ >> 4) & 0xF];
  foo[1] = hex[hexaDigit_ & 0xF];
  const char *foo_ = foo;
  printf(foo_);
}

void log(/*int line_,*/ const char *function_, const char *file_,
         const char *message_) {
  printf("[");
  // printf(to_string(line_));
  printf(" ");
  printf(function_);
  printf(" ");
  printf(file_);
  printf(" ]: ");
  log(message_);
}

void log(const char *message_, const logLevel level_) {
  switch (level_) {
  case logLevel::INFO:
    printf("[INFO]: ", color::CYAN_ON_BLACK);
    break;
  case logLevel::DEBUG:
    printf("[DEBUG]: ", color::PURPLE_ON_BLACK);
    break;
  case logLevel::WARNING:
    printf("[WARNING]: ", color::YELLOW_ON_BLACK);
    break;
  case logLevel::FATAL:
    printf("[FATAL]: ", color::LIGHT_RED_ON_BLACK);
    break;
  case logLevel::TRACE:
    printf("[TRACE]: ", color::WHITE_ON_BLACK);
    break;
  case logLevel::ERROR:
    printf("[ERROR]: ", color::RED_ON_BLACK);
    break;
  }
  printf(message_, color::WHITE_ON_BLACK);
  printf("\n");
}
} // namespace core
} // namespace falconOS