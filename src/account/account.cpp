#include <account.h>

bool Account::SetPhoneNumber(std::string new_number){
    if (new_number.size() != PHONE_NUMBER_LENGTH) {
        return false;
    }
    phone_number = new_number;
    return true;
}

std::string Account::GetPhoneNumber() const {
    return phone_number;
}
bool Account::SetPin(std::string new_pin) {
    if (new_pin.size() != PIN_LENGTH) {
        return false;
    }
    pin = new_pin;
    return true;
}

std::string Account::GetPin() const {
    return pin;
}