#pragma once

#include <optional>

#include <tan.h>

// # Screens
// Login
// Register
// ?

namespace Interface {
    inline std::optional<int> DisplayScreen() {
        std::optional<int> interface_result = std::nullopt;
        while (!(interface_result)) { // Waits until the interface result returns something
            interface_result = DisplayMainScreen();
        }
        return interface_result; // Passed onto main.cpp so it knows how to exit
    }
}
