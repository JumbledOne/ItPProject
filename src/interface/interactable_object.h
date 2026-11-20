#pragma once
#include <string>
#include <interface_object.h>

class InteractableObject : InterfaceObject {
public:
    virtual std::string Display();
    virtual bool ReceiveInput(char key); // Returns true if the input was used. Forwarded from Menu
};
