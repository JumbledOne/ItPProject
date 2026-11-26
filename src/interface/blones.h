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

    // Asks for the PIN of the account. Returns true if it was entered correctly (within PIN_ATTEMPTS tries)
    // Returns false if the line was left empty or the tries ran out (that is shown first), so the caller just goes back
    inline bool AskForPin(const Account& account, const string& prompt) {
        for (int attempt = 0; attempt < PIN_ATTEMPTS; attempt++) {
            cout << prompt;
            string input;
            InterfaceUtils::ReadLineTrimmed(input);
            if (input.empty()) {
                return false;
            }
            if (Account::IsValidPin(input) && input == account.GetPin()) {
                return true;
            }
            cout << "Incorrect PIN.\n";
        }
        cout << "Too many incorrect attempts. Press any key to continue...";
        InterfaceUtils::AwaitInput();
        return false;
    }

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
        if (!AskForPin(*account, "Enter your current PIN: ")) {
            return;
        }
        string input;

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

    // Deletes the account that is logged in together with its transaction history
    // The PIN has to be entered and then entered again as the confirmation. Leaving a line empty returns to the wallet
    inline void AccountDeletion(Session& session) {
        session.RequestScreen(SCREENS::WALLET); // Goes back to the wallet unless the account gets deleted
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }

        InterfaceUtils::ClearScreen();
        cout << "Deleting Account...\n";
        cout << "This permanently deletes the account of " << account->name << " and its transaction history.\n";
        if (account->GetMoney() > 0) {
            cout << "The balance of " << InterfaceUtils::FloatToPeso(account->GetMoney()) << " will be lost.\n";
        }
        cout << "Leave a line empty to go back without deleting.\n\n";

        if (!AskForPin(*account, "Enter your " + to_string(PIN_LENGTH) + "-DIGIT PIN: ")) {
            return;
        }

        string confirmation;
        cout << "Confirm your PIN: ";
        InterfaceUtils::ReadLineTrimmed(confirmation);
        if (confirmation.empty()) {
            return;
        }
        if (confirmation != account->GetPin()) {
            cout << "The PIN does not match so the account was not deleted. Press any key to continue...";
            InterfaceUtils::AwaitInput();
            return;
        }

        // Delete here
        int id = account->GetId();
        string name = account->name;
        session.LogOut(); // The active account is stored in the database so it has to be let go before it's deleted
        session.database->DeleteAccount(id);
        session.RequestScreen(SCREENS::MAIN);
        cout << "The account of " << name << " has been deleted. Press any key to continue...";
        InterfaceUtils::AwaitInput();
    }
}