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
        Transaction* transaction;
        const std::vector<Transaction*>& transaction_list = session.active_account->GetTransactions();

        // Page logic
        int page = 0;
        int page_count = (int)(ceil((float)transaction_list.size() / (float)TRANSACTIONS_PER_PAGE));

        while (true) {
            // Display transactions
            for (int i = 0; i < TRANSACTIONS_PER_PAGE; i++) {
                // Get transaction
                transaction = transaction_list[(page * TRANSACTIONS_PER_PAGE) + i];
                // Display type specific info
                switch (transaction->type) {
                    case TRANSACTION_TYPE::TRANSFER:
                        cout << "Transfer";
                        if (transaction->from && transaction->to) {
                            cout << "\nFrom: " << transaction->from->name;
                            cout << "\nTo: " << transaction->from->name;
                        }
                        break;
                    case TRANSACTION_TYPE::CASH_IN:
                        cout << "Cash In";
                        break;
                    case TRANSACTION_TYPE::CASH_OUT:
                        cout << "Cash Out";
                        break;
                }
                cout << "\nReference Number: " << transaction->reference_number;
                cout << "\nAmount: " << transaction->amount;
                cout << "\n";
            }
            
            // Display page control
            cout << "\n[" << page << " / " << page_count << "]";
            cout << "\n[1] Previous Page";
            cout << "\n[2] Next Page";
            cout << "\n[3] Return";

            // Listen for input
            int input;
            cin >> input;
            switch (input) {
                case 1:
                    if (page > 0) {
                        page--;
                    }
                    break;
                case 2:
                    if (page < page_count) {
                        page++;
                    }
                    break;
                case 3:
                    return;
                    break;
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