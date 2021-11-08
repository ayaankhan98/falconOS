#include <events/mouse_event_handler.hpp>

namespace falconOS {
namespace drivers {

MouseEventHandler::MouseEventHandler() {}

MouseEventHandler::~MouseEventHandler() {}

void MouseEventHandler::onActivate() {}

void MouseEventHandler::onMouseKeyPressed(
    falconOS::core::types::int32_t x, falconOS::core::types::int32_t y,
    falconOS::core::types::uint8_t button) {}
void MouseEventHandler::onMouseKeyPressed(falconOS::core::types::uint8_t) {}

void MouseEventHandler::onMouseKeyReleased(
    falconOS::core::types::int32_t x, falconOS::core::types::int32_t y,
    falconOS::core::types::uint8_t button) {}
void MouseEventHandler::onMouseKeyReleased(falconOS::core::types::uint8_t) {}

void MouseEventHandler::onMouseMove(falconOS::core::types::int32_t x,
                                    falconOS::core::types::int32_t y) {}
void MouseEventHandler::onMouseMove(falconOS::core::types::int32_t oldX,
                                    falconOS::core::types::int32_t oldY,
                                    falconOS::core::types::int32_t newX,
                                    falconOS::core::types::int32_t newY) {}
} // namespace drivers
} // namespace falconOS