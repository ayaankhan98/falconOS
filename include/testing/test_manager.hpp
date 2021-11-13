#pragma once

#include <testing/test.hpp>
#include <core/types.h>

namespace falconOS {
    namespace test {
        class TestManager {
            private : 
                Test *test_[5];
                char *command_[5];
                falconOS::core::types::uint8_t maxTests;
            public : 
                void registerTest(Test *test, char *command);
                void activateTest(char *command);
        };
    }
}