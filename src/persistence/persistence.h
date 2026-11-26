#pragma once

#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <string>
#include <system_error>
#include <cctype>
#include <cmath>

#include <session.h>
#include <database.h>
#include <interface_utils.h>
#include <parser.h>

const std::string FILE_NAME = "e_peso.txt";

namespace Persistence {

    // The names the parser reads back
    inline const char* TypeToString(TRANSACTION_TYPE type) {
        switch (type) {
            case TRANSACTION_TYPE::CASH_IN: return "CASH_IN";
            case TRANSACTION_TYPE::CASH_OUT: return "CASH_OUT";
            case TRANSACTION_TYPE::BILL_PAYMENT: return "BILL_PAYMENT";
            case TRANSACTION_TYPE::LOAD_PURCHASE: return "LOAD_PURCHASE";
            default: return "TRANSFER";
        }
    }

    // Saves the database to disk
    // Return value is whether it succeeded
    // It writes to a temporary file first and only then replaces the save so a crash while saving can't corrupt it
    inline bool Save(Session& session) {
        const std::string temp_name = FILE_NAME + ".tmp";
        std::ofstream file(temp_name);
        if (!file.is_open()) {
            return false;
        }

        // Serialize database values
        file << "DATABASE";
        file <<	"\n\tLastAccountID " << session.database->GetLastAccountId();
	    file << "\n\tLastTransactionID " << session.database->GetLastTransactionId();
        file << "\n";

        // Serialize accounts
        // Money and amounts are written by the same function that formats currency on the screens so they always agree
        for (const auto& [id, account] : session.database->GetAccounts()) {
            file << "ACCOUNT";
            file << "\n\tAccountNumber " << account.GetId();
            file << "\n\tName " << account.name;
            file << "\n\tPhoneNumber " << account.GetPhoneNumber();
            file << "\n\tPIN " << account.GetPin();
            file << "\n\tMoney " << InterfaceUtils::FormatCurrency(account.GetMoney());
            file << "\n";
        }

        // Serialize transactions
        // Accounts are saved by account number (-1 if there is none) so the transaction can be loaded before the account
        for (const auto& [id, transaction] : session.database->GetTransactions()) {
            int from = transaction.from ? transaction.from->GetId() : -1;
            int to = transaction.to ? transaction.to->GetId() : -1;
            file << "TRANSACTION";
            file << "\n\tReferenceNumber " << transaction.reference_number;
            file << "\n\tType " << TypeToString(transaction.type);
            file << "\n\tFrom " << from;
            file << "\n\tTo " << to;
            file << "\n\tAmount " << InterfaceUtils::FormatCurrency(transaction.amount);
            file << "\n";
        }

        file.close();
        std::error_code error;
        if (file.fail()) {
            std::filesystem::remove(temp_name, error);
            return false;
        }
        std::filesystem::rename(temp_name, FILE_NAME, error); // Replaces the old save
        if (error) {
            std::filesystem::remove(temp_name, error);
            return false;
        }
        return true;
    }

    // Loads data onto the database class directly
    // Return value is whether it succeeded
    // If it didn't succeed -> Make the default starting values
    inline bool Load(Session& session) {
        // Binary so that a Ctrl+Z byte in a name isn't treated as the end of the file. The parser skips the \r of Windows line endings
        std::ifstream file(FILE_NAME, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        Parser::ParseSaveFile(file, session);

        file.close();
        // A save without any accounts is the same as no save. The last ids were still restored so the new ones won't collide
        return !session.database->GetAccounts().empty();
    }
}
