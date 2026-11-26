#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <database.h>

namespace Persistence {
    // Saves the database to disk
    inline void Save(Session& session) {
        std::ofstream file("e_peso.txt");
        if (!file.is_open()) {
            return;
        }

        // Serialize database values
        file << "DATABASE";
        file <<	"\n\tLastAccountID " << -1;		
	    file << "\n\tLastTransactionID " << -1;
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
        file.close();
    }

    // Loads data onto the database class directly
    inline void Load(Session& session) {

    }
}
