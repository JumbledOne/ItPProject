#include <iostream>
#include <memory>
#include <optional>

#include <user.h>
#include <persistence.h>
#include <database.h>
#include <interface.h>

using namespace std;

// Global vars
std::unique_ptr<User> user;
std::unique_ptr<Database> database;

void LoadOrInitialize() {
    user = std::make_unique<User>();
    database = std::make_unique<Database>(); // Create database class. Empty but it'll be filled on load
    Persistence::Load(*database);
}


int main() {
    LoadOrInitialize();

    // Display Screen
    std::optional<int> interface_result = std::nullopt;
    while (!(interface_result)) { // Waits until the interface result returns something
       interface_result = Interface::DisplayScreen(); 
    }

    // Exit
    Persistence::Save(*database);
    return interface_result.value_or(0); // Returns whatever the interface code did. Defaults to zero
}