#include <session.h>

Session::Session() {
    database = std::make_unique<Database>();
}

void Session::RequestScreen(SCREENS new_screen) {
    screen = new_screen;
}
void Session::RequestExit(int new_exit_code) {
    if (exit_code == std::nullopt) { // Forces you to only be able to exit once
        exit_code = new_exit_code;
    }
}