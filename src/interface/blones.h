#pragma once

#include <iostream>
#include <string>

#include <session.h>
#include <transaction.h>

namespace Blones {
    
    using namespace std;

    constexpr int TRANSACTIONS_PER_PAGE = 3;

    void ViewTransactions(Session& session) {
        // Check first if the account even exists
        if (!session.active_account) {
            return;
        }
        system("cls"); // Clear screen first
        // Now the actual logic
        int start_index = 0;
        while (true) {
            // Display transactions
            for (Transaction* transaction : session.active_account->GetTransactions()) {
                cout << "Reference Number: " << transaction->reference_number;
                // Only display from and to if it's a transfer
                // And check if both accounts exists
                if (transaction->type == TRANSACTION_TYPE::TRANSFER &&
                    session.database->HasAccount(transaction->from) &&
                    session.database->HasAccount(transaction->to)) { 
                }
                // Amount
                cout << "Amount: " << transaction->amount;
                cout << "From" << transaction->from
            }

        }
    }

    void ViewAccountInformation(Session& session) {
        // Check first if the account even exists
        if (!session.active_account) {
            return;
        }
        system("cls"); // Clear screen first
        
    }
}