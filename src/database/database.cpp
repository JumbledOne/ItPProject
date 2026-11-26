#include <database.h>

Account& Database::CreateAccount(const std::string& name, float initial_balance) {
    int id = ++last_account_id; // Increment first so the first account gets id 0
    Account& account = accounts[id]; // Apparently this constructs it automatically
    account.SetId(id);
    account.name = name;
    account.SetMoney(initial_balance);
    return account;
}

Transaction& Database::CreateTransaction(Account* from, Account* to, float amount, TRANSACTION_TYPE type) {
    int ref = ++last_transaction_id;
    // Create transaction object
    Transaction& transaction = transactions[ref];
    transaction.reference_number = ref;
    transaction.from = from;
    transaction.to = to;
    transaction.amount = amount;
    transaction.type = type;
    // Link transaction to accounts. This is also what moves the money
    if (from) {
        from->LinkTransaction(transaction);
    }
    if (to && to != from) { // Don't link it twice if it's the same account
        to->LinkTransaction(transaction);
    }

    return transaction;
}

// NOTE: Session's active account points into this map. Call Session::LogOut() first if this is the active account
void Database::DeleteAccount(int id) {
    if (!HasAccount(id)) {
        return;
    }
    // Transactions point to accounts so clear those before the account is gone
    Account* account = &accounts.at(id);
    for (auto& [ref, transaction] : transactions) {
        if (transaction.from == account) transaction.from = nullptr;
        if (transaction.to == account) transaction.to = nullptr;
    }
    accounts.erase(id);
}

void Database::DeleteTransaction(int id) {
    if (!HasTransaction(id)) {
        return;
    }
    const Transaction& transaction = transactions.at(id);
    // Unlinking also gives the money back
    if (transaction.from) {
        transaction.from->UnlinkTransaction(id);
    }
    if (transaction.to) {
        transaction.to->UnlinkTransaction(id);
    }
    transactions.erase(id);
}

Account& Database::GetAccount(int id) {
    return accounts.at(id);
}

Transaction& Database::GetTransaction(int id) {
    return transactions.at(id);
}

bool Database::HasAccount(int id) {
    return accounts.count(id) == 1;
}

bool Database::HasTransaction(int id) {
     return transactions.count(id) == 1;
}

const std::unordered_map<int, Account>& Database::GetAccounts() const {
    return accounts;
}

const std::unordered_map<int, Transaction>& Database::GetTransactions() const {
    return transactions;
}

int Database::GetLastAccountId() const {
    return last_account_id;
}

int Database::GetLastTransactionId() const {
    return last_transaction_id;
}