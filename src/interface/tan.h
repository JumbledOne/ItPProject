#pragma once

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <conio.h>
#include <optional>

#include <session.h>
#include <interface_utils.h>

namespace Tan {
    using namespace std;

    // These stay global because they are shared between screens (and Ev.h reads them)
    inline int activeAccount;
    inline double RegisteredWallet, PresetWallet = 5000;

    inline void DisplayMainScreen(Session& session) {
        system("cls");
        cout << "Welcome to ePESO!\n\n";
        cout << "[1] Log In\n[2] Register\n[3] Exit\nChoose option: ";
        int opt;
        cin >> opt;
        switch (opt) {
            case 1: session.RequestScreen(SCREENS::LOGIN); break;
            case 2:
                RegisteredWallet = 0;
                session.RequestScreen(SCREENS::REGISTRATION);
                break;
            case 3:
                cout << "Exiting program...";
                session.RequestExit(0);
                break;
            default:
                cout << "Invalid option. Press any key to continue...";
                _getch();
                break;
        }
    }

    inline void Register(Session& session) {
        system("cls");
        cout << "Welcome to ePESO!\n[CREATING ACCOUNT]\n\n";
        long long accountNum, accNum, pw;
        int PIN, attempt;

        // Account Number input
        cout << "Enter account number (10 Digits): ";
        cin >> accountNum;
        if (to_string(accountNum).size() > InterfaceUtils::REFERENCE_NUMBER_LENGTH) {
            cout << "Invalid account number. Press any key to try again.";
            _getch();
            session.RequestScreen(SCREENS::MAIN);
            return;
        }
        if (session.database->HasAccount(accountNum)) {
            cout << "Account already exists. Press any key to try again.";
            _getch();
            session.RequestScreen(SCREENS::MAIN);
            return;
        }

        // Pin Inputs
        cout << "Enter a 4-DIGIT PIN: ";
        cin >> PIN;
        if (PIN < 999) {
            cout << "Invalid PIN. Press any key to try again.";
            _getch();
            session.RequestScreen(SCREENS::MAIN);
            return;
        }

        // Pin confirmation
        cout << "Confirm PIN: ";
        cin >> pw;
        if (pw != PIN) {
            cout << "PIN does not match. Press any key to try again.";
            _getch();
            session.RequestScreen(SCREENS::MAIN);
            return;
        }

        cout << "Account created! Press any key to continue...";
        _getch();
        system("cls");
        cout << "Welcome to ePESO!\nLogging In...\n\nEnter account number: ";
        cin >> accNum;
        if (accNum == accountNum) {
            attempt = 3;
            do {
                cout << "Enter PIN: ";
                cin >> pw;
                if (pw == PIN) {
                    cout << "Access Granted! Enter any key to continue...";
                    _getch();
                    activeAccount = 0;
                    session.RequestScreen(SCREENS::WALLET);
                    return;
                }
                cout << "Incorrect PIN.\n";
                --attempt;
            } while (attempt > 0);
            cout << "Access Denied. Exiting Program.";
        } else {
            cout << "Account number not registered. Press any key to continue...";
            _getch();
        }
        session.RequestScreen(SCREENS::MAIN);
    }

    inline void Login(Session& session) {
        InterfaceUtils::ClearScreen();

        // Check first if there are any accounts
        const auto& account_list = session.database->GetAccounts();
        if (account_list.empty()) {
            cout << "No accounts registered. Press any key to continue...";
            _getch();
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
        cin >> account_num_input;
        // Check if the account inputted exists
        if (session.database->HasAccount(account_num_input)) {
            Account& account = session.database->GetAccount(account_num_input);
            int attempt = 3;
            int pin_input;
            do {
                cout << "Enter PIN: ";
                cin >> pin_input;
                if (pin_input == account.GetPin()) {
                    cout << "Access Granted. Enter any key to continue...";
                    _getch();
                    activeAccount = 1;
                    session.active_account = &account;
                    session.RequestScreen(SCREENS::WALLET);
                    return;
                }
                cout << "Incorrect PIN.\n";
                --attempt;
            } while (attempt > 0);
            cout << "Access Denied. Exiting Program.";
        } else {
            cout << "Account number not registered. Press any key to continue...";
            _getch();
        }
        session.RequestScreen(SCREENS::MAIN);
    }

    // Wallet menu. Each action screen sets the screen back to WALLET before doing its work,
    // so returning from it lands here again.
    inline void Wallet(Session& session) {
        InterfaceUtils::ClearScreen();

        // Display account info
        cout << "Welcome to ePESO!\n";
        cout << "\nName: " << session.active_account->name;
        cout << "\nWallet Amount: PHP " << fixed << setprecision(2)
             << (activeAccount == 1 ? PresetWallet : RegisteredWallet);

        // Input guide
        cout << "\n[1] Cash In";
        cout << "\n[2] Cash Out";
        cout << "\n[3] Pay Bills";
        cout << "\n[4] Buy Load";
        cout << "\n[5] View Transactions";
        cout << "\n[6] Log Out";
        cout << "\n" << InterfaceUtils::OPTION_PROMPT;

        // Input
        int walletOpt;
        cin >> walletOpt;
        switch (walletOpt) {
            case 1: session.RequestScreen(SCREENS::CASH_IN); break;
            case 2: session.RequestScreen(SCREENS::CASH_OUT); break;
            case 3: session.RequestScreen(SCREENS::PAY_BILLS); break;
            case 4: session.RequestScreen(SCREENS::BUY_LOAD); break;
            case 5: session.RequestScreen(SCREENS::VIEW_TRANSACTIONS); break;
            case 6: session.RequestScreen(SCREENS::MAIN); break;
            default:
                cout << "Invalid option. Press any key to try again.";
                _getch();
                break;
        }
    }

    inline void CashIn(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        system("cls");
        cout << "Welcome to ePESO!\n[CASH IN]\n";
        if (activeAccount == 1)
            cout << "Wallet Amount: PHP " << fixed << setprecision(2) << PresetWallet;
        else
            cout << "Wallet Amount: PHP " << fixed << setprecision(2) << RegisteredWallet;
        cout << "\nEnter amount to cash in: ";
        double cash;
        cin >> cash;
        if (cash <= 50000 && cash > 0) {
            cout << "Press any key to confirm cash in...";
            _getch();
            if (activeAccount == 1) PresetWallet += cash;
            else RegisteredWallet += cash;
        } else {
            cout << "Invalid amount. Please press any key to try again.";
            _getch();
        }
    }

    inline void CashOut(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        system("cls");
        cout << "Welcome to ePESO!\n\n[CASH OUT]\n";
        if (activeAccount == 1)
            cout << "Wallet Amount: PHP " << fixed << setprecision(2) << PresetWallet;
        else
            cout << "Wallet Amount: PHP " << fixed << setprecision(2) << RegisteredWallet;
        cout << "\nEnter amount to cash out: ";
        double cash;
        cin >> cash;
        double balance = activeAccount == 1 ? PresetWallet : RegisteredWallet;
        if (cash > 0 && cash <= balance) {
            cout << "Press any key to confirm cash out...";
            _getch();
            if (activeAccount == 1) PresetWallet -= cash;
            else RegisteredWallet -= cash;
        } else {
            cout << "Invalid amount. Please press any key to try again.";
            _getch();
        }
    }
}
