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
    std::string pin; // A string so that it can start with a zero. Empty means no PIN was set
    std::vector<Transaction*> transactions;
    float money = 0;

public:
    std::string name; // The name can be anything so this can be set directly

    void SetId(int new_id);
    int GetId() const;

    float GetMoney() const;
    void SetMoney(float new_money); // Only for the initial value. After that the money only changes through LinkTransaction and UnlinkTransaction
    bool HasSufficientBalance(float amount) const; // Read only check. Does nothing to the money

    bool SetPhoneNumber(std::string new_number); // Return value is whether this succeeded
    std::string GetPhoneNumber() const;

    // A valid PIN is exactly PIN_LENGTH digits. Use this to check input before it gets to the setter
    static bool IsValidPin(const std::string& pin);
    bool SetPin(const std::string& new_pin); // Return value is whether this succeeded. Fails if the PIN isn't valid
    std::string GetPin() const;

    // Transactions are tracked here so you can easily iterate over them
    // Called from database which owns account and transaction
    // Linking also moves the money: it's added if this account is the receiver and deducted if it's the sender
    // Unlinking undoes that
    const std::vector<Transaction*>& GetTransactions() const;
    void LinkTransaction(Transaction& transaction);
    void UnlinkTransaction(int reference_number);
};