#pragma once

#include <iostream>
#include <string>

#include <session.h>
#include <interface_utils.h>

namespace Tan {
    using namespace std;

    constexpr float MAX_CASH_IN = 50000.0f;

    inline void Pause() {
        cout << "\nPress any key to continue...";
        InterfaceUtils::AwaitInput();
    }

    // Mobile numbers are PHONE_NUMBER_LENGTH digits and start with 09
    inline bool IsValidPhoneNumber(const string& phone) {
        return phone.size() == PHONE_NUMBER_LENGTH &&
               phone.find_first_not_of("0123456789") == string::npos &&
               phone.substr(0, 2) == "09";
    }

    // Returns the account id that has this mobile number, or -1 if there isn't one
    inline int FindPhone(Session& session, const string& phone) {
        for (const auto& [id, account] : session.database->GetAccounts()) {
            if (account.GetPhoneNumber() == phone) {
                return id;
            }
        }
        return -1;
    }

    inline void DisplayMainScreen(Session& session) {
        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n\n";
        cout << "[1] Log In\n[2] Register\n[3] Exit\n" << InterfaceUtils::OPTION_PROMPT;
        int opt;
        if (!(cin >> opt)) {
            InterfaceUtils::ClearInput();
            opt = 0; // Goes to the invalid option case
        }
        switch (opt) {
            case 1: session.RequestScreen(SCREENS::LOGIN); break;
            case 2: session.RequestScreen(SCREENS::REGISTRATION); break;
            case 3:
                cout << "Exiting program...";
                session.RequestExit(0);
                break;
            default:
                cout << "Invalid option. Press any key to continue...";
                InterfaceUtils::AwaitInput();
                break;
        }
    }

    inline void Register(Session& session) {
        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n[CREATING ACCOUNT]\n\n";
        string PIN, confirm_PIN;

        // Name input
        string name_input;
        cout << "Enter your name: ";
        InterfaceUtils::ClearInput(); // Drop the Enter left over from the menu choice
        getline(cin, name_input);

        // Mobile number input
        string phone;
        while (true) {
            cout << "Enter your mobile number (" << PHONE_NUMBER_LENGTH << " digits, starts with 09): ";
            cin >> phone;
            if (!IsValidPhoneNumber(phone)) {
                cout << "Invalid mobile number. Press any key to try again.\n";
                InterfaceUtils::AwaitInput();
                continue;
            }
            if (FindPhone(session, phone) != -1) {
                cout << "That mobile number is already registered. Press any key to try again.\n";
                InterfaceUtils::AwaitInput();
                continue;
            }
            break;
        }

        // Pin Inputs
        while (true) {
            cout << "Enter a " << PIN_LENGTH << "-DIGIT PIN: ";
            cin >> PIN;
            if (!Account::IsValidPin(PIN)) {
                InterfaceUtils::ClearInput(); // Throw away bad input
                cout << "Invalid PIN. Press any key to try again.\n";
                InterfaceUtils::AwaitInput();
                continue;
            }
            break;
        }

        // Pin confirmation
        while (true) {
            cout << "Confirm PIN: ";
            cin >> confirm_PIN;
            if (cin.fail() || confirm_PIN != PIN) {
                InterfaceUtils::ClearInput();
                cout << "PIN does not match. Press any key to try again.\n";
                InterfaceUtils::AwaitInput();
                continue;
            }
            break;
        }

        // Only create the account once everything is valid so a failed attempt can't leave one behind
        Account& account = session.database->CreateAccount(name_input, 0);
        account.SetPhoneNumber(phone);
        account.SetPin(PIN);

        // Now log in
        session.LogIn(account);
        cout << "Account created! Your account number is " << InterfaceUtils::FormatReferenceNumber(account.GetId())
             << ". Press any key to continue...";
        InterfaceUtils::AwaitInput();
        session.RequestScreen(SCREENS::WALLET);
    }

    inline void Login(Session& session) {
        InterfaceUtils::ClearScreen();

        // Check first if there are any accounts
        const auto& account_list = session.database->GetAccounts();
        if (account_list.empty()) {
            cout << "No accounts registered. Press any key to continue...";
            InterfaceUtils::AwaitInput();
            session.RequestScreen(SCREENS::MAIN);
            return;
        }

        cout << "Welcome to ePESO!\nLogging In...\n\n";
        cout << "Accounts:\n";
        for (const auto& [id, listed_account] : account_list) {
            cout << "\t" << InterfaceUtils::FormatReferenceNumber(id) << " - " << listed_account.name << "\n";
        }

        // Listen for Input
        cout << "\nEnter account number: ";
        int account_num_input;
        if (!(cin >> account_num_input)) {
            InterfaceUtils::ClearInput();
            account_num_input = -1; // Can't match an account
        }
        // Check if the account inputted exists
        if (session.database->HasAccount(account_num_input)) {
            Account& account = session.database->GetAccount(account_num_input);
            int attempt = 3;
            string pin_input;
            do {
                cout << "Enter PIN: ";
                cin >> pin_input;
                InterfaceUtils::ClearInput(); // Drop anything extra on the line
                if (Account::IsValidPin(pin_input) && pin_input == account.GetPin()) {
                    cout << "Access Granted. Enter any key to continue...";
                    InterfaceUtils::AwaitInput();
                    session.LogIn(account);
                    session.RequestScreen(SCREENS::WALLET);
                    return;
                }
                cout << "Incorrect PIN.\n";
                --attempt;
            } while (attempt > 0);
            cout << "Access Denied. Press any key to continue...";
            InterfaceUtils::AwaitInput();
        } else {
            cout << "Account number not registered. Press any key to continue...";
            InterfaceUtils::AwaitInput();
        }
        session.RequestScreen(SCREENS::MAIN);
    }

    // Wallet menu. Each action screen sets the screen back to WALLET before doing its work,
    // so returning from it lands here again.
    inline void Wallet(Session& session) {
        Account* account = session.GetActiveAccount();
        if (!account) {
            session.RequestScreen(SCREENS::MAIN);
            return;
        }
        InterfaceUtils::ClearScreen();

        // Display account info
        cout << "Welcome to ePESO!\n";
        cout << "\nName: " << account->name;
        cout << "\nAccount Number: " << InterfaceUtils::FormatReferenceNumber(account->GetId());
        if (!account->GetPhoneNumber().empty()) {
            cout << "\nMobile Number: " << account->GetPhoneNumber();
        }
        cout << "\nWallet Amount: " << InterfaceUtils::FloatToPeso(account->GetMoney());

        // Input guide
        cout << "\n[1] Check Balance";
        cout << "\n[2] Cash In";
        cout << "\n[3] Cash Out";
        cout << "\n[4] Send Money";
        cout << "\n[5] Pay Bills";
        cout << "\n[6] Buy Load";
        cout << "\n[7] View Transactions";
        cout << "\n[8] Log Out";
        cout << "\n" << InterfaceUtils::OPTION_PROMPT;

        // Input
        int walletOpt;
        if (!(cin >> walletOpt)) {
            InterfaceUtils::ClearInput();
            walletOpt = 0; // Goes to the invalid option case
        }
        switch (walletOpt) {
            case 1:
                cout << "Available balance: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << "\n";
                Pause();
                break;
            case 2: session.RequestScreen(SCREENS::CASH_IN); break;
            case 3: session.RequestScreen(SCREENS::CASH_OUT); break;
            case 4: session.RequestScreen(SCREENS::SEND_MONEY); break;
            case 5: session.RequestScreen(SCREENS::PAY_BILLS); break;
            case 6: session.RequestScreen(SCREENS::BUY_LOAD); break;
            case 7: session.RequestScreen(SCREENS::VIEW_TRANSACTIONS); break;
            case 8:
                session.LogOut();
                session.RequestScreen(SCREENS::MAIN);
                break;
            default:
                cout << "Invalid option. Press any key to try again.";
                InterfaceUtils::AwaitInput();
                break;
        }
    }

    inline void CashIn(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }
        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n[CASH IN]\n";
        cout << "Wallet Amount: " << InterfaceUtils::FloatToPeso(account->GetMoney());
        cout << "\nEnter amount to cash in (max " << InterfaceUtils::FloatToPeso(MAX_CASH_IN) << "): ";
        float amount;
        if (!InterfaceUtils::ReadAmount(amount) || amount > MAX_CASH_IN) {
            cout << "Invalid amount.";
            Pause();
            return;
        }
        char confirm;
        cout << "Confirm? (Y/N): ";
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            // Creating the transaction is what adds the money
            session.database->CreateTransaction(nullptr, account, amount, TRANSACTION_TYPE::CASH_IN);
            cout << "Cash in successful. New balance: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << "\n";
        } else {
            cout << "Cash in cancelled or failed.\n";
        }
        Pause();
    }

    inline void CashOut(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }
        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n[CASH OUT]\n";
        cout << "Wallet Amount: " << InterfaceUtils::FloatToPeso(account->GetMoney());
        cout << "\nEnter amount to cash out: ";
        float amount;
        if (!InterfaceUtils::ReadAmount(amount) || !account->HasSufficientBalance(amount)) {
            cout << "Invalid amount or insufficient funds.";
            Pause();
            return;
        }
        char confirm;
        cout << "Confirm? (Y/N): ";
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            // Creating the transaction is what takes the money out. The balance was already checked above
            session.database->CreateTransaction(account, nullptr, amount, TRANSACTION_TYPE::CASH_OUT);
            cout << "Cash out successful. New balance: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << "\n";
        } else {
            cout << "Cash out cancelled or failed.\n";
        }
        Pause();
    }
}
