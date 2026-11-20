#pragma once
#include <string>

class InterfaceObject {
public:
    int pos_x;
    int pos_y;
    int width = 10;
    
    virtual std::string GetDisplayString() const;
    virtual bool ApplyInput(char key); // Returns true if the input was used
};
