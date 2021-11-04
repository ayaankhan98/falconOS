#pragma once

#include<core/types.h>

namespace falconOS {
    namespace libc {
void memset(falconOS::core::types::uint8_t *dest, falconOS::core::types::uint8_t val, falconOS::core::types::uint32_t len);
    }
}