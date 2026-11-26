#include <database.h>

Account& Database::CreateAccount() {
    int id = ++last_account_id; // Increment first so the first account gets id 0
    Account& account = accounts[id]; // Apparently this constructs it automatically
    account.SetId(id);
    return account;
}

Transaction& Database::CreateTransaction(int from, int to, float amount) {
    int ref = ++last_transaction_id;
    // Create transaction object
    Transaction& transaction = transactions[ref];
    transaction.reference_number = ref;
    transaction.from = from;
    transaction.to = to;
    transaction.amount = amount;
    // Link transaction to accounts
    if (HasAccount(from)) {
        GetAccount(from).AddTransaction(transaction);
    }
    if (to != from && HasAccount(to)) { // Don't link it twice if it's the same account
        GetAccount(to).AddTransaction(transaction);
    }

    return transaction;
}

// NOTE: Session::active_account points into this map. Set it to nullptr first if this is the active account
void Database::DeleteAccount(int id) {
    accounts.erase(id);
}

void Database::DeleteTransaction(int id) {
    if (!HasTransaction(id)) {
        return;
    }
    const Transaction& transaction = transactions.at(id);
    if (HasAccount(transaction.from)) {
        GetAccount(transaction.from).DeleteTransaction(id);
    }
    if (HasAccount(transaction.to)) {
        GetAccount(transaction.to).DeleteTransaction(id);
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