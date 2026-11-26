#pragma once

#include <unordered_map>
#include <optional>
#include <functional>

#include <screens.h>
#include <session.h>
#include <tan.h>

namespace Interface {
    inline const std::unordered_map<SCREENS, std::function<void(Session&)>> SCREEN_TO_FUNCTION = {
        {SCREENS::MAIN, DisplayMainScreen},
        {SCREENS::REGISTRATION, Register}
    };

    // The main function that runs the interface on a loop
    // Runs until session.RequestExit() then main reads session.exit_code
    inline void ShowInterface(Session& session) {
        while (!session.exit_code) {
            SCREEN_TO_FUNCTION.at(session.screen)(session);
        }
    }
}
