#include <parser_proxies.h>

#include <cmath>

namespace Parser {
    // DATABASE PROXY
    void DatabaseProxy::RunCommand(const std::string& command, const std::string& value) {
        if (command == "LastAccountID") {
            last_account_id = std::stoi(value);
        } else if (command == "LastTransactionID") {
            last_transaction_id = std::stoi(value);
        }
    }

    void DatabaseProxy::Realize(Session& session) {
        // Nothing is required. The setters only ever raise the ids so a missing or corrupted one changes nothing
        session.database->SetLastAccountId(last_account_id);
        session.database->SetLastTransactionId(last_transaction_id);
        realized = true;
    }

    // ACCOUNT PROXY
    void AccountProxy::RunCommand(const std::string& command, const std::string& value) {
        if (command == "AccountNumber") {
            id = std::stoi(value);
        } else if (command == "Name") {
            name = value;
        } else if (command == "PhoneNumber") {
            phone_number = value;
        } else if (command == "PIN") {
            pin = value;
        } else if (command == "Money") {
            money = std::stof(value);
        }
    }

    void AccountProxy::Realize(Session& session) {
        // CreateAccount needs the account number to restore and a name
        if (id < 0 || name.empty()) {
            return;
        }
        // The rest is optional. A corrupted value falls back to a default instead of cancelling
        float initial_balance = (std::isfinite(money) && money >= 0) ? money : 0;
        Account& account = session.database->CreateAccount(name, initial_balance, id);
        account.SetPhoneNumber(phone_number); // The setters ignore what isn't valid
        account.SetPin(pin);
        realized = true;
    }

    void AccountProxy::ApplyBalance(Session& session) const {
        // Looked up by id instead of keeping a pointer since a later account with the same id replaces this one
        // A corrupted balance is left alone so the account keeps what the transactions gave it
        if (realized && std::isfinite(money) && money >= 0 && session.database->HasAccount(id)) {
            session.database->GetAccount(id).SetMoney(money);
        }
    }

    // TRANSACTION PROXY
    void TransactionProxy::RunCommand(const std::string& command, const std::string& value) {
        if (command == "ReferenceNumber") {
            id = std::stoi(value);
        } else if (command == "Type") {
            if (value == "TRANSFER") type = TRANSACTION_TYPE::TRANSFER;
            else if (value == "CASH_IN") type = TRANSACTION_TYPE::CASH_IN;
            else if (value == "CASH_OUT") type = TRANSACTION_TYPE::CASH_OUT;
            else if (value == "BILL_PAYMENT") type = TRANSACTION_TYPE::BILL_PAYMENT;
            else if (value == "LOAD_PURCHASE") type = TRANSACTION_TYPE::LOAD_PURCHASE;
        } else if (command == "From") {
            from = std::stoi(value);
        } else if (command == "To") {
            to = std::stoi(value);
        } else if (command == "Amount") {
            amount = std::stof(value);
        }
    }

    void TransactionProxy::Realize(Session& session) {
        // CreateTransaction needs the reference number to restore and the amount
        if (id < 0 || !std::isfinite(amount) || amount <= 0) {
            return;
        }
        // An account that doesn't exist is the same as there being none
        Database& database = *session.database;
        Account* from_account = (from >= 0 && database.HasAccount(from)) ? &database.GetAccount(from) : nullptr;
        Account* to_account = (to >= 0 && database.HasAccount(to)) ? &database.GetAccount(to) : nullptr;
        // It also needs an account to belong to
        if (!from_account && !to_account) {
            return;
        }
        database.CreateTransaction(from_account, to_account, amount, type, id);
        realized = true;
    }
}
