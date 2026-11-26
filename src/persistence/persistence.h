#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#include <session.h>
#include <database.h>

const std::string FILE_NAME = "e_peso.txt";

namespace Persistence {

    struct AccountProxy {
        int id;
        int name;
    }

    struct TransactionProxy {
        int id;
        int 
    }

    // Saves the database to disk
    // Return value is whether it succeeded
    inline bool Save(Session& session) {
        std::ofstream file(FILE_NAME);
        if (!file.is_open()) {
            return false;
        }

        // Serialize database values
        file << "DATABASE";
        file <<	"\n\tLastAccountID " << session.database->GetLastAccountId();
	    file << "\n\tLastTransactionID " << session.database->GetLastTransactionId();
        file << "\n";

        // Serialize accounts
        for (const auto& [id, account] : session.database->GetAccounts()) {
            file << "ACCOUNT";
            file << "\n\tAccountNumber " << account.GetId();
            file << "\n\tName " << account.name;
            file << "\n\tPhoneNumber " << account.GetPhoneNumber();
            file << "\n\tPIN " << account.GetPin();
            file << "\n\tMoney " << account.GetMoney();
            file << "\n";
        }

        // Serialize transactions
        // Accounts are saved by account number (-1 if there is none) so the transaction can be loaded before the account
        for (const auto& [id, transaction] : session.database->GetTransactions()) {
            int from = transaction.from ? transaction.from->GetId() : -1;
            int to = transaction.to ? transaction.to->GetId() : -1;
            file << "TRANSACTION";
            file << "\n\tReferenceNumber " << transaction.reference_number;
            file << "\n\tFrom " << from;
            file << "\n\tTo " << to;
            file << "\n\tAmount " << transaction.amount;
            file << "\n";
        }

        file.close();
        return !file.fail();
    }

    // Loads data onto the database class directly
    // Return value is whether it succeeded
    // If it didn't succeed -> Make the default starting values
    inline bool Load(Session& session) {
        std::ifstream file(FILE_NAME);
        if (!file.is_open()) {
            return false;
        }

        std::array<std::string, 2> command; // [0] is the command, [1] is the value
        bool reading_value = false; // False while still reading the command

        std::string token = "";
        char character;
        while (file.get(character)) {
            // New lines clear the command array
            if (character == '\n') {
                command[0] = "";
                command[1] = "";
            }
            if (std::isspace(character)) {
                token = "";
            }
        }

        file.close();
        return true;
    }
}
