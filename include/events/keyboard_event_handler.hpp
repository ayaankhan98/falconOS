#pragma once

namespace falconOS {
namespace drivers {
class KeyboardEventHandler {
public:
  KeyboardEventHandler();
  ~KeyboardEventHandler();

  virtual void onKeyPressed(char);
  virtual void onKeyReleased(char);
};
} // namespace drivers
} // namespace falconOS