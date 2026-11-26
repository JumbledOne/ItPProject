#pragma once

#include <unordered_map>

#include <account.h>
#include <transaction.h>

// Stores accounts and transactions
class Database {
private:
    // Maps account no. and ref no. to accounts and transactions respectively
    // The classes keep their own copy of these values so that they can be read directly without having to rely on the database object
    std::unordered_map<int, Account> accounts;
    std::unordered_map<int, Transaction> transactions;

    int last_account_id = -1; // When a new account is made, they get the id last_account_id + 1
    int last_transaction_id = -1; // Same as above
    
public:
    // These below just handle the creation and deletion. Handle it at the caller
    Account& CreateAccount();
    Transaction& CreateTransaction(int from, int to, float amount);

    void DeleteAccount(int id);
    void DeleteTransaction(int id);

    Account& GetAccount(int id);
    Transaction& GetTransaction(int id);
};