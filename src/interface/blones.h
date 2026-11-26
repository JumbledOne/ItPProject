#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <session.h>
#include <transaction.h>
#include <interface_utils.h>

namespace Blones {
    
    using namespace std;

    constexpr int TRANSACTIONS_PER_PAGE = 3;

    void ViewTransactions(Session& session) {
        // Check first if the account even exists
        if (!session.active_account) {
            return;
        }
        // Now the actual logic
        const std::vector<Transaction*>& transaction_list = session.active_account->GetTransactions();
        int page = 0;
        int page_count = (int)(ceil((float)transaction_list.size() / (float)TRANSACTIONS_PER_PAGE));
        if (page_count == 0) {
            page_count = 1; // Always at least one page
        }

        while (true) {
            InterfaceUtils::ClearScreen();
            // Display transactions
            for (int i = 0; i < TRANSACTIONS_PER_PAGE; i++) {
                // Check first if the index goes beyond the list size
                // If so break so that the last page doesn't contain all transactions
                int index = (page * TRANSACTIONS_PER_PAGE) + i;
                if (index >= (int)transaction_list.size()) {
                    break;
                }
                const Transaction* transaction = transaction_list[index];

                // Display type specific info
                // First line acts as a header and intentionally doesn't contain a line break
                switch (transaction->type) {
                    case TRANSACTION_TYPE::TRANSFER:
                        cout << "Transfer";
                        if (transaction->from && transaction->to) {
                            cout << "\nFrom: " << transaction->from->name;
                            cout << "\nTo: " << transaction->to->name;
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
                cout << "\nAmount: " << InterfaceUtils::FloatToPeso(transaction->amount);
                cout << "\n";
            }
            
            // Display page control
            cout << "\n[" << (page + 1) << " / " << page_count << "]";
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
                    if (page < page_count - 1) {
                        page++;
                    }
                    break;
                case 3:
                    return;
            }
        }
    }

    void ViewAccountInformation(Session& session) {
        // Check first if the account even exists
        if (!session.active_account) {
            return;
        }
        while (true) {
            InterfaceUtils::ClearScreen();
            // Display Info
            cout << "Account: " << session.active_account->name;
            cout << "\nBalance: " << InterfaceUtils::FloatToPeso(session.active_account->GetMoney());
            cout << "\n";

            // Input Guide
            cout << "[1] Change Name";
            cout << "[2] View Transactions";
            cout << "[3] Return To Menu";

            // Handle Input
            int input;
            cin >> input;
            switch (input) {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    
                    break;
            }
        }
    }
}