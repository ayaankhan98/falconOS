#include <events/keyboard_event_handler.hpp>

namespace falconOS {
namespace drivers {

KeyboardEventHandler::KeyboardEventHandler() {}

KeyboardEventHandler::~KeyboardEventHandler() {}

void KeyboardEventHandler::onKeyPressed(char) {}

void KeyboardEventHandler::onKeyReleased(char) {}

} // namespace drivers
} // namespace falconOS