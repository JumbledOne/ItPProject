#include <database.h>

Account& Database::CreateAccount() {
    int id = ++last_account_id; // Increment first so the first account gets id 0
    Account& account = accounts[id]; // Apparently this constructs it automatically
    account.SetId(id);
    return account;
}

Transaction& Database::CreateTransaction(int from, int to, float amount) {
    int ref = ++last_transaction_id;
    Transaction& transaction = transactions[ref];
    transaction.reference_number = ref;
    transaction.from = from;
    transaction.to = to;
    transaction.amount = amount;
    return transaction;
}

void Database::DeleteAccount(int id) {
    accounts.erase(id);
}

void Database::DeleteTransaction(int id) {
    transactions.erase(id);
}

Account& Database::GetAccount(int id) {
    return accounts.at(id);
}

const Account& Database::GetAccount(int id) const {
    return accounts.at(id);
}

Transaction& Database::GetTransaction(int id) {
    return transactions.at(id);
}

const Transaction& Database::GetTransaction(int id) const {
    return transactions.at(id);
}
