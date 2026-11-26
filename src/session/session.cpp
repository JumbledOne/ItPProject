#include <session.h>

void Session::SwitchScreen(SCREENS new_screen) {
    if (new_screen != SCREENS::NONE) {
        screen = new_screen;
    }
}
void Session::RequestExit(int new_exit_code) {
    if (exit_code == std::nullopt) { // Forces you to only be able to exit once
        exit_code = new_exit_code;
    }
}