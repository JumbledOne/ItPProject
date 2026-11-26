#include <account.h>
#include <cmath>

// Can only be called once
void Account::SetId(int new_id) {
    if (id == -1) {
        id = new_id;
    }
}

int Account::GetId() const {
    return id;
}

float Account::GetMoney() const {
    return money;
}

bool Account::HasSufficientBalance(float amount) const {
    return std::isfinite(amount) && amount > 0 &&
           std::isfinite(money) && money >= amount;
}

// Adds a valid positive amount to the wallet balance.
bool Account::AddMoney(float amount) {
    if (!std::isfinite(amount) || amount <= 0 ||
        !std::isfinite(money) ||
        !std::isfinite(money + amount)) {
        return false;
    }

    money += amount;
    return true;
}

// Deducts money only when sufficient funds are available.
bool Account::DeductMoney(float amount) {
    if (!HasSufficientBalance(amount)) {
        return false;
    }

    money -= amount;
    return true;
}

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
