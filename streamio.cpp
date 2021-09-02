#include "streamio.h"

void printf(const char* str_, const color color_) {
  static uint16_t* videoMemory = (uint16_t*) VIDEO_MEMORY_ADDRESS;

  static uint8_t cursorX = 0, cursorY = 0;

  for(int32_t i = 0; str_[i] != '\0'; ++i) {
    switch(str_[i]) {
      case '\n':
        cursorY++;
        cursorX = 0;
        break;

      default:
        videoMemory[screen::COLUMNS * cursorY + cursorX] =
          (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0x00FF) | color_;
        videoMemory[screen::COLUMNS * cursorY + cursorX] =
          (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0xFF00) | str_[i];
        cursorX++;
        break;
    }

    if (cursorX >= screen::COLUMNS) {
      cursorY++;
      cursorX = 0;
    }

    if (cursorY >= screen::ROWS) {
      for(cursorY = 0; cursorY <= screen::ROWS; cursorY++) {
        for(cursorX = 0; cursorX <= screen::COLUMNS; cursorX++) {
          videoMemory[screen::COLUMNS * cursorY + cursorX] =
            (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0x00FF) |
            color::BLACK_ON_BLACK;
          videoMemory[screen::COLUMNS * cursorY + cursorX] =
            (videoMemory[screen::COLUMNS * cursorY + cursorX] & 0xFF00) | ' ';
        }
      }
      cursorX = 0, cursorY = 0;
    }
  }
}


void log(const char* message_, const logLevel level_) {
  switch(level_) {
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
      printf("[DEBUG]: ", color::LIGHT_RED_ON_BLACK);
      break;
    case logLevel::TRACE:
      printf("[DEBUG]: ", color::WHITE_ON_BLACK);
      break;
    case logLevel::ERROR:
      printf("[DEBUG]: ", color::RED_ON_BLACK);
      break;
  }
  printf(message_, color::WHITE_ON_BLACK);
  printf("\n");
}

