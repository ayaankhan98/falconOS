#include "mouse.h"
#include "streamio.h"
#include "base_string.h"

void switchColors(int8_t x, int8_t y) 
{
    static uint16_t* videoMemory = (uint16_t*) VIDEO_MEMORY_ADDRESS;
    /// Background color is set to foreground, and vice versa
    videoMemory[80 * y + x] = ((videoMemory[80 * y + x] & 0xF000) >> 4) | 
                              ((videoMemory[80 * y + x] & 0x0F00) << 4) |
                              (videoMemory[80 * y + x] & 0x00FF);
}

MouseDriver::MouseDriver (InterruptManager* manager)
: InterruptHandler(manager, 0x2c),
dataport(0x60),
commandport(0x64)
{
    offset = 0;
    buttons = 0;

    switchColors(40, 12);

    commandport.write(0xa8); // activate interrupts
    commandport.write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.read() | 2);
    commandport.write(0x60); // command 0x60 = set controller command byte
    dataport.write(status);
    commandport.write(0xd4);
    dataport.write(0xf4);
    dataport.read();
}

MouseDriver::~MouseDriver()
{
}

uint32_t MouseDriver::handleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.read();

    if(!(status & 0x20))
      return esp;

    static int8_t pos_x = 40, pos_y = 12;      /// 40, 12 because it may be initialized in the middle

    buffer[offset] = dataport.read();
    offset = (offset + 1) % 3;
    
    if(offset == 0) 
    {
      switchColors(pos_x, pos_y);

      pos_x += buffer[1];
      if(pos_x < 0) pos_x = 0;
      if(pos_x >= screen::COLUMNS) pos_x = screen::COLUMNS - 1;

      pos_y -= buffer[2];
      if(pos_y < 0) pos_y = 0;
      if(pos_y >= screen::ROWS) pos_y = screen::ROWS - 1;
      
      switchColors(pos_x, pos_y);

      for(uint8_t i = 0; i < 3; ++i) 
      {
        if((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i)))
        {
          switchColors(pos_x, pos_y);
          int8_t temp = (buffer[0] & (0x01 << i));
          switch(temp)
          {
            case 1:
              log("Left Key Pressed");
              break;
            case 2:
              log("Right Key Pressed");
              break;
            case 4:
              log("Scroll Key Pressed");
              break;
          }

        }
      }
      buttons = buffer[0];
    }

    return esp;
}
