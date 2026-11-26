#include <session.h>
#include <persistence.h>
#include <database.h>
#include <interface.h>

int main() {
    std::unique_ptr<Session> session = std::make_unique<Session>(); // Owns the database. Everything else reaches it through session.database
    Persistence::Load(*session);

    // Display Screen
    Interface::ShowInterface(*session); // Returns once a screen requests an exit

    // Exit
    Persistence::Save(*session);
    return session->exit_code.value_or(0); // Returns whatever the interface code did. Defaults to zero
}
