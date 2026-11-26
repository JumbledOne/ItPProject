#pragma once

#include <memory>
#include <optional>

#include <database.h>
#include <screens.h>

// Pass this around to classes that need it rather than making it a singleton
class Session {
private:
    Account* active_account = nullptr; // Not owned; points into database->accounts. Null means nobody is logged in

public:
    Session();

    std::unique_ptr<Database> database;
    SCREENS screen = SCREENS::MAIN;
    std::optional<int> exit_code = std::nullopt;

    // Change the active account through these so it isn't set directly
    void LogIn(Account& account); // Does not check the PIN. Check it at the caller
    void LogOut(); // Sets the active account to nullptr
    Account* GetActiveAccount() const; // Null means nobody is logged in

    void RequestScreen(SCREENS new_screen); // Only a request you still have to return manually
    void RequestExit(int new_exit_code); // Only a request you still have to return manually

};