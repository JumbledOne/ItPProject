#include <account.h>

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

bool Account::SetPin(int new_pin) {
    // PIN_LENGTH digits means 1000 to 9999 when it's 4
    int min_pin = 1;
    for (int i = 1; i < PIN_LENGTH; i++) {
        min_pin *= 10;
    }
    int max_pin = min_pin * 10 - 1;
    if (new_pin < min_pin || new_pin > max_pin) {
        return false;
    }
    pin = new_pin;
    return true;
}

int Account::GetPin() const {
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