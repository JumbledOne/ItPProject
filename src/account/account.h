#pragma once

#include <string>
#include <vector>
#include <transaction.h>

constexpr int PHONE_NUMBER_LENGTH = 11; // XXXX XXX XXXX
constexpr int PIN_LENGTH = 4;

class Account {
private:
    // These are private since the setters have extra logic and so you don't change them directly by mistake
    int id = -1;
    std::string phone_number; 
    std::string pin;
    std::vector<Transaction> transactions;
    float money = 0;

public:
    std::string name; // The name can be anything so this can be set directly

    void SetId(int new_id);
    int GetId() const;

    bool SetPhoneNumber(std::string new_number); // Return value is whether this succeeded
    std::string GetPhoneNumber() const;

    bool SetPin(std::string new_pin); // Return value is whether this succeeded
    std::string GetPin() const;
};