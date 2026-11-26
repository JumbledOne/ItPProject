#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <database.h>

const std::string FILE_NAME = "e_peso.txt";

namespace Persistence {
    // Saves the database to disk
    inline void Save(Session& session) {
        std::ofstream file(FILE_NAME);
        if (!file.is_open()) {
            return;
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
            file << "\n\tPIN" << account.GetPin();
            file << "\n\tMoney " << account.GetMoney();
            file << "\n";
        }

        // Serialize transactions
        for (const auto& [id, transaction] : session.database->GetTransactions()) {

        }

        //
        file.close();
    }

    // Loads data onto the database class directly
    // Return value is whether it succeeded
    // If it didn't succeed -> Make the default starting values
    inline bool Load(Session& session) {
        std::ifstream file(FILE_NAME);
        if (!file.is_open()) {
            return false;
        }
        file.close();
        return true;
    }
}
