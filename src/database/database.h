#pragma once

#include <unordered_map>

#include <account.h>
#include <transaction.h>

// The biggest account number or reference number that can be forced or restored (8 digits, see doc/save_file_format.txt)
// Bigger ones come from a corrupted save, and next to INT_MAX they would overflow the next id
constexpr int MAX_ID = 99999999;

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
    // force_id picks the id instead of using last id + 1. It can't be negative, so -1 means it isn't forced
    // One above MAX_ID is ignored the same way
    // If something already has that id it is deleted first (for a transaction that gives the money back)
    // The last id is raised to force_id if it was higher so new ones can't collide. Used for loading from a save
    Account& CreateAccount(const std::string& name, float initial_balance, int force_id = -1);
    Transaction& CreateTransaction(Account* from, Account* to, float amount, TRANSACTION_TYPE type = TRANSACTION_TYPE::TRANSFER, int force_id = -1); // Either can be null

    void DeleteAccount(int id);
    void DeleteTransaction(int id);

    Account& GetAccount(int id);
    Transaction& GetTransaction(int id);

    bool HasAccount(int id);
    bool HasTransaction(int id);

    // Read only
    const std::unordered_map<int, Account>& GetAccounts() const;
    const std::unordered_map<int, Transaction>& GetTransactions() const;

    int GetLastAccountId() const;
    int GetLastTransactionId() const;

    // Only for the loading system, to restore the ids from the save (the last ones could have been deleted)
    // They can only raise the id. A lower one would let new ids collide with existing ones, so it's ignored
    void SetLastAccountId(int id);
    void SetLastTransactionId(int id);
};