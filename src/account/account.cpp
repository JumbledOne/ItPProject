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

// The only way the money changes after the initial value is by linking or unlinking a transaction
// This doesn't check anything. Check HasSufficientBalance before creating a transaction that takes money out
void Account::LinkTransaction(Transaction& transaction) {
    transactions.push_back(&transaction);
    if (transaction.to == this) {
        money += transaction.amount;
    }
    if (transaction.from == this) {
        money -= transaction.amount;
    }
}

// Does nothing if this account doesn't have that transaction
void Account::UnlinkTransaction(int reference_number) {
    for (size_t i = 0; i < transactions.size(); i++) {
        const Transaction* transaction = transactions[i];
        if (transaction->reference_number == reference_number) {
            // Undo what linking did
            if (transaction->to == this) {
                money -= transaction->amount;
            }
            if (transaction->from == this) {
                money += transaction->amount;
            }
            transactions.erase(transactions.begin() + i);
            break;
        }
    }
}

bool Account::HasSufficientBalance(float amount) const {
    return std::isfinite(amount) && amount > 0 && std::isfinite(money) && money >= amount;
}
