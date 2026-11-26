#pragma once

#include <memory>
#include <optional>

#include <database.h>
#include <screens.h>

// Pass this around to classes that need it rather than making it a singleton
class Session {
public:
    Session();

    std::unique_ptr<Database> database;
    int account = -1; // Uses the account id so that it doesn't need to know about the account class
    SCREENS screen = SCREENS::MAIN;
    std::optional<int> exit_code = std::nullopt;

    void RequestScreen(SCREENS new_screen); // Only a request you still have to return manually
    void RequestExit(int new_exit_code); // Only a request you still have to return manually
};