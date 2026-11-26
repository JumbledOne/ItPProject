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