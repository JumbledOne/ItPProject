#pragma once

#include <vector>

#include <account.h>
#include <transaction.h>

// Stores accounts and transactions
class Database {
private:
    std::vector<Account> accounts;
    std::vector<Transaction> transactions;

};