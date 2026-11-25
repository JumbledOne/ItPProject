#include <iostream>
#include <memory>
#include <optional>

#include <tan.h>
#include <user.h>

using namespace std;

// Global vars
std::unique_ptr<User> user;

void InitializeGlobals() {
    user = std::make_unique<User>();
}

int main() {
    InitializeGlobals();

    // Display Screen
    std::optional<int> interface_result = std::nullopt;
    while (!(interface_result)) { // Waits until the interface result returns something
       interface_result = DisplayMainScreen(); 
    }

    // Exit
    return interface_result.value_or(0); // Returns whatever the interface code did. Defaults to zero
}