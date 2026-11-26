#pragma once

#include <unordered_map>
#include <optional>
#include <functional>

#include <screens.h>
#include <session.h>
#include <tan.h>

// # Screens
// Login
// Register
// ?

namespace Interface {
    inline const std::unordered_map<SCREENS, std::function<std::optional<int>(Session&)>> SCREEN_TO_FUNCTION = {
        {SCREENS::MAIN, DisplayMainScreen},
        {SCREENS::REGISTRATION, Register}
    };

    // The main function that runs the interface on a loop
    inline std::optional<int> ShowInterface(Session& session) {
        std::optional<int> interface_result = std::nullopt;
        SCREENS last_screen = SCREENS::NONE;
        while (!(interface_result)) { // Waits until the interface result returns something
            if (session.screen != last_screen) {
                last_screen = session.screen;
                interface_result = SCREEN_TO_FUNCTION.at(session.screen)(session);
            }
        }
        return interface_result; // Passed onto main.cpp so it knows how to exit
    }
}
