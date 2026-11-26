#include <Ev.h>

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

extern int activeAccount;
extern double RegisteredWallet, PresetWallet;

// ill js add a function for "_getch()" cus it wont run on my device
void _getch() {
    cin.ignore(1000, '\n');
    cin.get();
}

//MY PART (PAY BILLS & BUY LOAD)
void PayBills(Session& session) {
    int billerChoice;
    string biller, billingNumber;
    double amount;
    char confirm;

    cout << "Welcome to ePESO!\n[BILLS PAYMENT]\n\n";
    cout << "[1] Electricity\n[2] Water\n[3] Internet\nChoose a biller: ";
    cin >> billerChoice;
    if (cin.fail()) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid choice. Press any key to continue..."; _getch();
        return;
    }
    switch (billerChoice) {
        case 1: biller = "Electricity"; break;
        case 2: biller = "Water"; break;
        case 3: biller = "Internet"; break;
        default:
            cout << "Invalid choice. Press any key to continue..."; _getch();
            return;
    }
    cout << "Enter fictional billing number: "; cin >> billingNumber;
    cout << "Enter payment amount: PHP "; cin >> amount;
    if (cin.fail() || amount <= 0) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid amount. Press any key to continue..."; _getch();
        return;
    }
    double balance = activeAccount == 1 ? PresetWallet : RegisteredWallet;
    if (amount > balance) {
        cout << "Insufficient balance. Press any key to continue..."; _getch();
        return;
    }
    cout << "Pay PHP " << fixed << setprecision(2) << amount << " to "
         << biller << "? (Y/N): ";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Payment cancelled. Press any key to continue..."; _getch();
        return;
    }

    if (activeAccount == 1) PresetWallet -= amount;
    else RegisteredWallet -= amount;
    cout << "\n=== PAYMENT RECEIPT ===\n"
         << "Biller: " << biller << '\n'
         << "Billing number: " << billingNumber << '\n'
         << "Amount paid: PHP " << amount << '\n'
         << "Remaining balance: PHP " << (activeAccount == 1 ? PresetWallet : RegisteredWallet) << '\n';
    //input Transaction history dito
    cout << "Press any key to continue..."; _getch();
}

void BuyLoad(Session& session) {
    int networkChoice, loadChoice, loadAmount;
    string network, mobileNumber;
    char confirm;

    cout << "Welcome to ePESO!\n[BUY MOBILE LOAD]\n\n";
    cout << "[1] Globe\n[2] Smart\n[3] DITO\nChoose a network: ";
    cin >> networkChoice;
    if (cin.fail()) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid choice. Press any key to continue..."; _getch();
        return;
    }
    switch (networkChoice) {
        case 1: network = "Globe"; break;
        case 2: network = "Smart"; break;
        case 3: network = "DITO"; break;
        default:
            cout << "Invalid choice. Press any key to continue..."; _getch();
            return;
    }
    cout << "Enter recipient mobile number: "; cin >> mobileNumber;
    if (mobileNumber.size() != 11 || mobileNumber[0] != '0' || mobileNumber[1] != '9') {
        cout << "Enter an 11-digit number starting with 09. Press any key...";
        _getch(); return;
    }
    for (char digit : mobileNumber) {
        if (digit < '0' || digit > '9') {
            cout << "Invalid mobile number. Press any key to continue...";
            _getch(); return;
        }
    }
    cout << "[1] PHP 50\n[2] PHP 100\n[3] PHP 200\nChoose load amount: ";
    cin >> loadChoice;
    if (cin.fail()) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Invalid choice. Press any key to continue..."; _getch();
        return;
    }
    switch (loadChoice) {
        case 1: loadAmount = 50; break;
        case 2: loadAmount = 100; break;
        case 3: loadAmount = 200; break;
        default:
            cout << "Invalid choice. Press any key to continue..."; _getch();
            return;
    }
    double balance = activeAccount == 1 ? PresetWallet : RegisteredWallet;
    if (loadAmount > balance) {
        cout << "Insufficient balance. Press any key to continue..."; _getch();
        return;
    }
    cout << "Buy PHP " << loadAmount << ' ' << network << " load for "
         << mobileNumber << "? (Y/N): ";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Purchase cancelled. Press any key to continue..."; _getch();
        return;
    }

    if (activeAccount == 1) PresetWallet -= loadAmount;
    else RegisteredWallet -= loadAmount;
    cout << "\n=== LOAD RECEIPT ===\n"
         << "Network: " << network << '\n'
         << "Mobile number: " << mobileNumber << '\n'
         << "Load purchased: PHP " << fixed << setprecision(2) << loadAmount << '\n'
         << "Remaining balance: PHP " << (activeAccount == 1 ? PresetWallet : RegisteredWallet) << '\n';
    //Transaction history ulet dito
    cout << "Press any key to continue..."; _getch();
}
