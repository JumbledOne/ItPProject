#include <session.h>
#include <persistence.h>
#include <database.h>
#include <interface.h>

void CreateDefaultValues() {

}

int main() {
    std::unique_ptr<Session> session = std::make_unique<Session>(); // Owns the database. Everything else reaches it through session.database
    bool load_result = Persistence::Load(*session);
    if (!load_result) {
        CreateDefaultValues();
    }

    // Display Screen
    Interface::ShowInterface(*session); // Returns once a screen requests an exit

    // Exit
    Persistence::Save(*session);
    return session->exit_code.value_or(0); // Returns whatever the interface code did. Defaults to zero
}
