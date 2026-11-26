#pragma once

#include <optional>

#include <screens.h>
#include <account.h>

// Pass this around to classes that need it rather than making it a singleton
class Session {
public:
    int account = 0;
    SCREENS screen = SCREENS::NONE;
    std::optional<int> exit_code = std::nullopt;

    void SwitchScreen(SCREENS new_screen); // Can't be NONE
    void RequestExit(int new_exit_code);
};