#pragma once

#include <string>

constexpr int PHONE_NUMBER_LENGTH = 11; // XXXX XXX XXXX
constexpr int PIN_LENGTH = 4;

class Account {
private:
    // These are private so that you are forced to use the relevant accessors
    std::string phone_number; 
    std::string pin;

public:
    std::string name; // The name can be anything so this can be set directly

    bool SetPhoneNumber(std::string new_number); // Return value is whether this succeeded
    void GetPhoneNumber() const;
    bool SetPin(std::string new_pin); // Return value is whether this succeeded
    void GetPin() const;
};