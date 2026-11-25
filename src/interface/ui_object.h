#pragma once
#include <string>

class UiObject {
public:
    UiObject(int pos_x = 0, int pos_y = 0, int width = 10)
        : pos_x(pos_x), pos_y(pos_y), width(width) {}

    // Positioning
    int pos_x;
    int pos_y;
    int width = 10;
    
    virtual std::string GetDisplayString() const;
    virtual bool ApplyInput(char key); // Returns true if the input was used
};
