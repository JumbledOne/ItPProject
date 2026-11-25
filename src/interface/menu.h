#pragma once

#include <string>
#include <vector>
#include <ui_object.h>
#include <interactable_object.h>

// Handles displaying interface objects
// Can have multiple menus that handle different tasks
class Menu {
private:
    // The screen itself
    std::vector<std::string> screen;

    // Storing interaction states
    InteractableObject* focused;

    // Storing interface objects
    std::vector<UiObject> ui_objects;
    std::vector<InteractableObject> interactable_objects;

    // Helper functions for handling interface objects
    void DisplayUiObject(UiObject& ui_object);
    void ClearScreen();

public:
    // Screen Control
    Menu(int width, int height);
    void Refresh();

    // Input
    void ReceiveInput(char key); // Forward this to interface

    // Object placement. Called only when the menu is defined
    void PlaceObject(UiObject& ui_object, bool set_focused = false);
    void FocusObject(InteractableObject& interactable_object);
};
