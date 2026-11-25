#pragma once

#include <optional>

#include <screens.h>
#include <session.h>
#include <tan.h>

// # Screens
// Login
// Register
// ?

namespace Interface {
    inline std::optional<int> DisplayScreen(Session& session) {
        std::optional<int> interface_result = std::nullopt;
        while (!(interface_result)) { // Waits until the interface result returns something
            interface_result = DisplayMainScreen();
        }
        return interface_result; // Passed onto main.cpp so it knows how to exit
    }
}
