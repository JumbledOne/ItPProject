#pragma once

#include <iostream>
#include <string>

#include <session.h>
#include <interface_utils.h>
#include <tan.h> // For Tan::IsValidPhoneNumber

namespace Ev {
    using namespace std;

    // Shows the message then waits so it can be read before the wallet screen clears it
    inline void Fail(const string& message) {
        cout << message << " Press any key to continue...";
        InterfaceUtils::AwaitInput();
    }

    //MY PART (PAY BILLS & BUY LOAD)
    inline void PayBills(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }
        int billerChoice;
        string biller, billingNumber;
        float amount;
        char confirm;

        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n[BILLS PAYMENT]\n\n";
        cout << "Wallet Amount: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << "\n\n";
        cout << "[1] Electricity\n[2] Water\n[3] Internet\nChoose a biller: ";
        if (!(cin >> billerChoice)) {
            InterfaceUtils::ClearInput();
            Fail("Invalid choice.");
            return;
        }
        switch (billerChoice) {
            case 1: biller = "Electricity"; break;
            case 2: biller = "Water"; break;
            case 3: biller = "Internet"; break;
            default:
                Fail("Invalid choice.");
                return;
        }
        cout << "Enter billing number: ";
        cin >> billingNumber;
        cout << "Enter payment amount: ";
        if (!InterfaceUtils::ReadAmount(amount)) {
            Fail("Invalid amount.");
            return;
        }
        if (!account->HasSufficientBalance(amount)) {
            Fail("Insufficient balance.");
            return;
        }
        cout << "Pay " << InterfaceUtils::FloatToPeso(amount) << " to " << biller << "? (Y/N): ";
        cin >> confirm;
        if (confirm != 'Y' && confirm != 'y') {
            Fail("Payment cancelled.");
            return;
        }

        // Creating the transaction is what takes the money out
        const Transaction& transaction = session.database->CreateTransaction(account, nullptr, amount, TRANSACTION_TYPE::BILL_PAYMENT);
        cout << "\n=== PAYMENT RECEIPT ===\n"
            << "Reference number: " << InterfaceUtils::FormatReferenceNumber(transaction.reference_number) << '\n'
            << "Biller: " << biller << '\n'
            << "Billing number: " << billingNumber << '\n'
            << "Amount paid: " << InterfaceUtils::FloatToPeso(amount) << '\n'
            << "Remaining balance: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << '\n';
        cout << "Press any key to continue...";
        InterfaceUtils::AwaitInput();
    }

    inline void BuyLoad(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        Account* account = session.GetActiveAccount();
        if (!account) {
            return;
        }
        int networkChoice, loadChoice;
        float loadAmount;
        string network, mobileNumber;
        char confirm;

        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n[BUY MOBILE LOAD]\n\n";
        cout << "Wallet Amount: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << "\n\n";
        cout << "[1] Globe\n[2] Smart\n[3] DITO\nChoose a network: ";
        if (!(cin >> networkChoice)) {
            InterfaceUtils::ClearInput();
            Fail("Invalid choice.");
            return;
        }
        switch (networkChoice) {
            case 1: network = "Globe"; break;
            case 2: network = "Smart"; break;
            case 3: network = "DITO"; break;
            default:
                Fail("Invalid choice.");
                return;
        }
        cout << "Enter recipient mobile number: ";
        cin >> mobileNumber;
        if (!Tan::IsValidPhoneNumber(mobileNumber)) {
            Fail("Enter an 11-digit mobile number starting with 09.");
            return;
        }
        cout << "[1] PHP 50\n[2] PHP 100\n[3] PHP 200\nChoose load amount: ";
        if (!(cin >> loadChoice)) {
            InterfaceUtils::ClearInput();
            Fail("Invalid choice.");
            return;
        }
        switch (loadChoice) {
            case 1: loadAmount = 50; break;
            case 2: loadAmount = 100; break;
            case 3: loadAmount = 200; break;
            default:
                Fail("Invalid choice.");
                return;
        }
        if (!account->HasSufficientBalance(loadAmount)) {
            Fail("Insufficient balance.");
            return;
        }
        cout << "Buy " << InterfaceUtils::FloatToPeso(loadAmount) << ' ' << network << " load for "
            << mobileNumber << "? (Y/N): ";
        cin >> confirm;
        if (confirm != 'Y' && confirm != 'y') {
            Fail("Purchase cancelled.");
            return;
        }

        // Creating the transaction is what takes the money out
        const Transaction& transaction = session.database->CreateTransaction(account, nullptr, loadAmount, TRANSACTION_TYPE::LOAD_PURCHASE);
        cout << "\n=== LOAD RECEIPT ===\n"
            << "Reference number: " << InterfaceUtils::FormatReferenceNumber(transaction.reference_number) << '\n'
            << "Network: " << network << '\n'
            << "Mobile number: " << mobileNumber << '\n'
            << "Load purchased: " << InterfaceUtils::FloatToPeso(loadAmount) << '\n'
            << "Remaining balance: " << InterfaceUtils::FloatToPeso(account->GetMoney()) << '\n';
        cout << "Press any key to continue...";
        InterfaceUtils::AwaitInput();
    }
} // namespace Ev
