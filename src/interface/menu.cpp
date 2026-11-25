#include <menu.h>
#include <iostream>

void Menu::ClearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void Menu::DisplayUiObject(UiObject& ui_object) {
}

Menu::Menu(int width, int height)
{
    screen = std::vector<std::string>(height, std::string(width, ' '));
    focused = nullptr;
}

void Menu::Refresh() {
    ClearScreen();
    for (auto& e : ui_objects) {
        DisplayUiObject(e);
    }
}

void Menu::ReceiveInput(char key) {
}

void Menu::PlaceObject(UiObject& ui_object, bool set_focused) {
}

void Menu::FocusObject(InteractableObject& interactable_object) {
}
