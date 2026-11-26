#include <account.h>
#include <cmath>
#include <limits>

#include <algorithm>

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

void Account::SetMoney(float new_money) {
    money = new_money;
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

bool Account::IsValidPin(const std::string& pin) {
    return pin.size() == PIN_LENGTH && pin.find_first_not_of("0123456789") == std::string::npos;
}

bool Account::SetPin(const std::string& new_pin) {
    if (!IsValidPin(new_pin)) {
        return false;
    }
    pin = new_pin;
    return true;
}

std::string Account::GetPin() const {
    return pin;
}

const std::vector<Transaction*>& Account::GetTransactions() const {
    return transactions;
}

void Account::AddTransaction(Transaction& transaction) {
    transactions.push_back(&transaction);
}

void Account::DeleteTransaction(int reference_number) {
    int i = 0;
    for (Transaction* transaction : transactions) {
        if (transaction->reference_number == reference_number) {
            transactions.erase(transactions.begin() + i);
            break;
        }
        i++;
    }
}
bool Account::HasSufficientBalance(float amount) const {
    return std::isfinite(amount) && amount > 0 && std::isfinite(money) && money >= amount;
}
bool Account::AddMoney(float amount) {
    if (!std::isfinite(amount) || amount <= 0 || !std::isfinite(money) ||
        amount > std::numeric_limits<float>::max() - money) return false;
    money += amount;
    return true;
}
bool Account::DeductMoney(float amount) {
    if (!HasSufficientBalance(amount)) return false;
    money -= amount;
    return true;
}
