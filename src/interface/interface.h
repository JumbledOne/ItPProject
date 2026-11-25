#pragma once

#include <optional>

#include <user.h>
#include <database.h>
#include <tan.h>

// # Screens
// Login
// Register
// ?

namespace Interface {
    inline std::optional<int> DisplayScreen(User& user, Database& database) {
        std::optional<int> interface_result = std::nullopt;
        while (!(interface_result)) { // Waits until the interface result returns something
            interface_result = DisplayMainScreen();
        }
        return interface_result; // Passed onto main.cpp so it knows how to exit
    }
}
