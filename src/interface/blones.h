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

    inline void ViewTransactions(Session& session) {
        // Check first if the account even exists
        if (!session.GetActiveAccount()) {
            session.RequestScreen(SCREENS::MAIN); // Otherwise this screen would just repeat forever
            return;
        }
        // Now the actual logic
        const std::vector<Transaction*>& transaction_list = session.GetActiveAccount()->GetTransactions();
        int page = 0;
        int page_count = (int)(ceil((float)transaction_list.size() / (float)TRANSACTIONS_PER_PAGE));
        if (page_count == 0) {
            page_count = 1; // Always at least one page
        }
        string message; // Shown under the page controls once, e.g. when the input wasn't a valid option

        while (true) {
            InterfaceUtils::ClearScreen();
            if (transaction_list.empty()) {
                cout << "No transactions yet.\n";
            }
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
                    case TRANSACTION_TYPE::BILL_PAYMENT:
                        cout << "Bill Payment";
                        break;
                    case TRANSACTION_TYPE::LOAD_PURCHASE:
                        cout << "Load Purchase";
                        break;
                }
                cout << "\nReference Number: " << InterfaceUtils::FormatReferenceNumber(transaction->reference_number);
                cout << "\nAmount: " << InterfaceUtils::FloatToPeso(transaction->amount);
                cout << "\n\n";
            }
            
            // Display page control
            cout << "\n[" << (page + 1) << " / " << page_count << "]";
            cout << "\n[1] Previous Page";
            cout << "\n[2] Next Page";
            cout << "\n[3] Return";
            if (message != "") {
                cout << "\n" << message;
                message = "";
            }

            // Listen for input
            cout << "\n" << InterfaceUtils::OPTION_PROMPT;
            int input;
            if (!InterfaceUtils::ReadValue(input)) {
                input = 0; // Goes to the invalid option case
            }
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
                    session.RequestScreen(SCREENS::WALLET);
                    return;
                default:
                    message = "Invalid option.";
                    break;
            }
        }
    }

    constexpr int PIN_ATTEMPTS = 3;

    // Leaving the line empty returns to the wallet without changing anything
    inline void ChangeName(Session& session) {
        session.RequestScreen(SCREENS::WALLET); // Always goes back to the wallet when it's done
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }

        InterfaceUtils::ClearScreen();
        cout << "Current name: " << account->name << "\n";
        cout << "Enter your new name (leave empty to go back): ";
        string new_name;
        InterfaceUtils::ReadLine(new_name);
        if (new_name.empty()) {
            return;
        }

        account->name = new_name;
        cout << "Your name has been changed. Press any key to continue...";
        InterfaceUtils::AwaitInput();
    }

    // The current PIN has to be entered first and the new one has to be entered twice
    // Leaving a line empty at any point returns to the wallet without changing anything
    inline void ChangePin(Session& session) {
        session.RequestScreen(SCREENS::WALLET); // Always goes back to the wallet when it's done
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }

        InterfaceUtils::ClearScreen();
        cout << "Leave a line empty to go back without changing your PIN.\n\n";

        // Check the current PIN first
        string input;
        bool is_verified = false;
        for (int attempt = 0; attempt < PIN_ATTEMPTS && !is_verified; attempt++) {
            cout << "Enter your current PIN: ";
            InterfaceUtils::ReadLineTrimmed(input);
            if (input.empty()) {
                return;
            }
            is_verified = Account::IsValidPin(input) && input == account->GetPin();
            if (!is_verified) {
                cout << "Incorrect PIN.\n";
            }
        }
        if (!is_verified) {
            cout << "Too many incorrect attempts. Press any key to continue...";
            InterfaceUtils::AwaitInput();
            return;
        }

        // Now the new one. If the confirmation doesn't match it starts over from the new PIN
        while (true) {
            string new_pin;
            while (true) {
                cout << "Enter your new " << PIN_LENGTH << "-DIGIT PIN: ";
                InterfaceUtils::ReadLineTrimmed(new_pin);
                if (new_pin.empty()) {
                    return;
                }
                if (!Account::IsValidPin(new_pin)) {
                    cout << "Invalid PIN. It must be exactly " << PIN_LENGTH << " digits.\n";
                } else if (new_pin == account->GetPin()) {
                    cout << "That is already your PIN.\n";
                } else {
                    break;
                }
            }

            cout << "Confirm your new PIN: ";
            InterfaceUtils::ReadLineTrimmed(input);
            if (input.empty()) {
                return;
            }
            if (input == new_pin) {
                account->SetPin(new_pin);
                cout << "Your PIN has been changed. Press any key to continue...";
                InterfaceUtils::AwaitInput();
                return;
            }
            cout << "The PINs do not match. Try again.\n";
        }
    }
}