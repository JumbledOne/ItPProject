#pragma once
#include <string>
#include <vector>
#include <interface_object.h>
#include <interactable_object.h>

// Handles displaying interface objects
// Can have multiple menus that handle different tasks
class Menu {
private:
    // The screen itself
    std::string screen;

    // Storing interaction states
    InteractableObject& focused;

    // Storing interface objects
    std::vector<InterfaceObject> interface_objects;
    std::vector<InteractableObject> interactable_objects;

    // Helper functions for handling interface objects
    void DisplayInterfaceObject(InterfaceObject& interface_object);

public:
    // Screen Control
    Menu(int width, int height);
    void Refresh();

    // Input
    void ReceiveInput(char key); // Forward this to interface

    // Object placement. Called only when the menu is defined
    void PlaceObject(InterfaceObject& interface_object, bool set_focused = false);
    void FocusObject(InteractableObject& interactable_object);
};