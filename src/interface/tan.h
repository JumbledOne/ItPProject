#pragma once
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cmath>
#include <conio.h>
#include <session.h>
#include <transfer.h>

namespace Tan {
using namespace std;
inline void Pause() { cout << "\nPress any key to continue..."; _getch(); }
inline void ClearInput() { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
inline bool ReadAmount(float& amount) {
    if (!(cin >> amount)) { ClearInput(); return false; }
    return isfinite(amount) && amount > 0;
}
inline Account* Current(Session& session) {
    if (session.account < 0 || !session.database->HasAccount(session.account)) return nullptr;
    return &session.database->GetAccount(session.account);
}
inline int FindPhone(Session& session, const string& phone) {
    for (const auto& entry : session.database->GetAccounts())
        if (entry.second.GetPhoneNumber() == phone) return entry.first;
    return -1;
}
inline void DisplayMainScreen(Session& session) {
    cout << "\nWelcome to ePESO!\n[1] Log In\n[2] Register\n[3] Exit\nChoose option: ";
    int choice;
    if (!(cin >> choice)) { ClearInput(); cout << "Invalid option.\n"; return; }
    switch(choice) {
        case 1: session.RequestScreen(SCREENS::LOGIN); break;
        case 2: session.RequestScreen(SCREENS::REGISTRATION); break;
        case 3: session.RequestExit(0); break;
        default: cout << "Invalid option.\n";
    }
}
inline void Register(Session& session) {
    session.RequestScreen(SCREENS::MAIN);
    string name, phone, pin, confirm;
    cout << "\n[CREATING ACCOUNT]\nFull name: ";
    getline(cin >> ws, name);
    cout << "Mobile number (11 digits): "; cin >> phone;
    cout << "4-digit PIN: "; cin >> pin;
    cout << "Confirm PIN: "; cin >> confirm;
    if (name.empty() || phone.size() != PHONE_NUMBER_LENGTH ||
        phone.find_first_not_of("0123456789") != string::npos ||
        phone.substr(0, 2) != "09" || pin.size() != PIN_LENGTH ||
        pin.find_first_not_of("0123456789") != string::npos || pin != confirm ||
        FindPhone(session, phone) != -1) {
        cout << "Invalid or duplicate account details.\n"; Pause(); return;
    }
    Account& user = session.database->CreateAccount();
    user.name = name;
    user.SetPhoneNumber(phone);
    user.SetPin(pin);
    cout << "Account created! Account ID: " << user.GetId() << "\n";
    Pause();
}
inline void Login(Session& session) {
    session.RequestScreen(SCREENS::MAIN);
    string phone, pin;
    cout << "\n[LOGIN]\nMobile number: "; cin >> phone;
    int id = FindPhone(session, phone);
    for (int tries = 0; tries < 3; ++tries) {
        cout << "PIN: "; cin >> pin;
        if (id >= 0 && session.database->GetAccount(id).GetPin() == pin) {
            session.account = id;
            session.RequestScreen(SCREENS::WALLET);
            return;
        }
        cout << "Incorrect credentials.\n";
    }
    Pause();
}
inline void Wallet(Session& session) {
    Account* user = Current(session);
    if (!user) { session.RequestScreen(SCREENS::MAIN); return; }
    cout << fixed << setprecision(2);
    cout << "\nWelcome to ePESO!\n" << user->name << " (Account ID " << user->GetId() << ")\n"
         << "Wallet Amount: PHP " << user->GetMoney() << "\n"
         << "[1] Check Balance\n[2] Cash In\n[3] Cash Out\n[4] Send Money\n"
         << "[5] Pay Bills\n[6] Buy Load\n[7] Log Out\nChoose option: ";
    int choice;
    if (!(cin >> choice)) { ClearInput(); cout << "Invalid option.\n"; return; }
    switch (choice) {
        case 1: cout << "Available balance: PHP " << user->GetMoney() << "\n"; Pause(); break;
        case 2: session.RequestScreen(SCREENS::CASH_IN); break;
        case 3: session.RequestScreen(SCREENS::CASH_OUT); break;
        case 4: session.RequestScreen(SCREENS::SEND_MONEY); break;
        case 5: session.RequestScreen(SCREENS::PAY_BILLS); break;
        case 6: session.RequestScreen(SCREENS::BUY_LOAD); break;
        case 7: session.account = -1; session.RequestScreen(SCREENS::MAIN); break;
        default: cout << "Invalid option.\n";
    }
}
inline void CashIn(Session& session) {
    session.RequestScreen(SCREENS::WALLET);
    Account* user = Current(session);
    if (!user) return;
    float amount;
    cout << "\n[CASH IN]\nAmount (max PHP 50000): ";
    if (!ReadAmount(amount) || amount > 50000) { cout << "Invalid amount.\n"; Pause(); return; }
    char confirm; cout << "Confirm? (Y/N): "; cin >> confirm;
    if ((confirm == 'Y' || confirm == 'y') && user->AddMoney(amount))
        cout << "Cash in successful. New balance: PHP " << fixed << setprecision(2) << user->GetMoney() << "\n";
    else cout << "Cash in cancelled or failed.\n";
    Pause();
}
inline void CashOut(Session& session) {
    session.RequestScreen(SCREENS::WALLET);
    Account* user = Current(session);
    if (!user) return;
    float amount;
    cout << "\n[CASH OUT]\nAmount: ";
    if (!ReadAmount(amount) || !user->HasSufficientBalance(amount)) {
        cout << "Invalid amount or insufficient funds.\n"; Pause(); return;
    }
    char confirm; cout << "Confirm? (Y/N): "; cin >> confirm;
    if ((confirm == 'Y' || confirm == 'y') && user->DeductMoney(amount))
        cout << "Cash out successful. New balance: PHP " << fixed << setprecision(2) << user->GetMoney() << "\n";
    else cout << "Cash out cancelled or failed.\n";
    Pause();
}
inline void SendMoney(Session& session) {
    session.RequestScreen(SCREENS::WALLET);
    Account* sender = Current(session);
    if (!sender) return;
    string phone;
    cout << "\n[SEND MONEY]\nAvailable balance: PHP " << fixed << setprecision(2)
         << sender->GetMoney() << "\nRecipient mobile number: "; cin >> phone;
    int recipientId = FindPhone(session, phone);
    if (recipientId < 0 || recipientId == session.account) {
        cout << "Recipient not found or self-transfer.\n"; Pause(); return;
    }
    cout << "Recipient: " << session.database->GetAccount(recipientId).name << "\n";
    float amount;
    cout << "Amount: PHP ";
    if (!ReadAmount(amount) || !sender->HasSufficientBalance(amount)) {
        cout << "Invalid amount or insufficient funds.\n"; Pause(); return;
    }
    char confirm; cout << "Confirm transfer? (Y/N): "; cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') { cout << "Cancelled.\n"; Pause(); return; }
    if (TransferMoney(*session.database, session.account, recipientId, amount)) {
        cout << "Transfer successful! Reference: " << session.database->GetLastTransactionId()
             << "\nRemaining balance: PHP " << sender->GetMoney() << "\n";
    } else cout << "Transfer failed.\n";
    Pause();
}
} // namespace Tan
