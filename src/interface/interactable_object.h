#pragma once
#include <string>
#include <ui_object.h>

class InteractableObject : public UiObject {
public:
    virtual std::string Display();
    virtual bool ReceiveInput(char key); // Returns true if the input was used. Forwarded from Menu
};
