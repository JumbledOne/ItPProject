#include <iostream>
#include <optional>

#include <session.h>
#include <persistence.h>
#include <database.h>
#include <interface.h>

using namespace std;

int main() {
    std::unique_ptr<Session> session = std::make_unique<Session>(); // Owns the database. Everything else reaches it through session.database
    Persistence::Load(*session);

    // Display Screen
    std::optional<int> interface_result = std::nullopt;
    while (!(interface_result)) { // Waits until the interface result returns something
       interface_result = Interface::ShowInterface(*session);
    }

    // Exit
    Persistence::Save(*session);
    return interface_result.value_or(0); // Returns whatever the interface code did. Defaults to zero
}
