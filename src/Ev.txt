#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <string>

using namespace std;

void Login();
void Register();
void CashIn();
void CashOut();
void PayBills();
void BuyLoad();

// ill js add a function for "_getch()" cus it wont run on my device 
void _getch() {
    cin.ignore(1000, '\n');
    cin.get();
}
//denise's program
int opt, walletOpt, PIN, attempt, activeAccount;
long long accountNum, accNum, pw;
double cash, RegisteredWallet, PresetWallet=5000;
int main(){
	do{
	system("cls");
	cout << "Welcome to ePESO!\n\n";
	cout << "[1] Log In\n";
	cout << "[2] Register\n";
	cout << "[3] Exit\n";
	cout << "Choose option: ";
	cin >> opt;
	switch(opt){
		case 1:
			Login();		
			break;
		case 2:
			RegisteredWallet=0;
			Register();
			break;
		case 3: 
			cout << "Exiting program...";
			return 0;
		default:
			cout <<"Invalid option. Press any key to continue...";
			_getch();
			break;
	}
	
	}while(opt!= 3);
	return 0;
}
void Register(){
	system("cls");
	cout << "Welcome to ePESO!\n[CREATING ACCOUNT]\n\n";
	cout << "Enter account number (10 Digits): ";
	cin >> accountNum;
	if(accountNum <999999999){
		cout<<"Invalid account number. Press any key to try again.";
		_getch();
		return;
	} 
	cout << "Enter a 4-DIGIT PIN: ";
	cin >> PIN;	
	if(PIN <999){
		cout<<"Invalid PIN. Press any key to try again.";
		_getch();
		return;
	} 
	cout << "Confirm PIN: ";
	cin >> pw;	
	if (pw!=PIN){
		cout<<"PIN does not match. Press any key to try again.";
		_getch();
		return;
	}
	cout << "Account created! Press any key to continue...";
	_getch();
	system("cls");
	cout << "Welcome to ePESO!\nLogging In...\n\n";
	cout << "Enter account number: ";
	cin >> accNum;
	if (accNum==accountNum){
		attempt=3;
		do {
			cout << "Enter PIN: ";
			cin >> pw;
			if (pw==PIN){
				cout << "Access Granted! Enter any key to continue...";
				_getch();
				activeAccount =0;
				do{
					system("cls");
					cout << "Welcome to ePESO!\n\nWallet Amount: PHP " <<fixed<<setprecision(2)<< RegisteredWallet;
					cout<<"\n[1] Cash In\n";
					cout <<"[2] Cash Out\n";
					cout << "[3] Pay Bills\n";
                    cout << "[4] Buy Load\n";
                    cout << "[5] Log Out\n";
					cout << "Choose option: ";
					cin >> walletOpt;
					switch (walletOpt){ //here i added paybills & buy load option
						            case 1:
                            CashIn();
                            break;
                        case 2:
                            CashOut();
                            break;
                        case 3:
                            PayBills();
                            break;
                        case 4:
                            BuyLoad();
                            break;
                        case 5:
                        break;
						default:
							cout<<"Invalid option. Press any key to try again.";
							_getch();
							break;
					}
										
				}while(walletOpt!=5);
				return;
			}
			else{
				cout <<"Incorrect PIN.\n";
				attempt--;
			}
			if(attempt==0){
				cout<<"Access Denied. Exiting Program.";
			}
		}while(attempt>0);
	} 
	else if (accNum!=accountNum) {
		cout<<"Account number not registered. Press any key to continue...";
		_getch();
	}
}



void Login() {
	system("cls");
	cout << "Welcome to ePESO!\nLogging In...\n\n";
	cout << "Enter account number: ";
	cin >> accountNum;
	if (accountNum==9215213192){
		attempt=3;
		do {
			cout << "Enter PIN: ";
			cin >> PIN;
			if (PIN==4444){
				cout << "Access Granted. Enter any key to continue...";
				_getch();
				activeAccount = 1;
					do{
						system("cls");
						cout << "Welcome to ePESO!\n\nWallet Amount: PHP " <<fixed<<setprecision(2)<< PresetWallet;
					    cout<<"\n[1] Cash In\n";
					    cout <<"[2] Cash Out\n";
					    cout << "[3] Pay Bills\n";
                        cout << "[4] Buy Load\n";
                        cout << "[5] Log Out\n";
						cout << "Choose option: ";
						cin >> walletOpt;
						switch (walletOpt){
						case 1:
                            CashIn();
                            break;
                        case 2:
                            CashOut();
                            break;
                        case 3:
                            PayBills();
                            break;
                        case 4:
                            BuyLoad();
                            break;
                        case 5:
                        break;
							default:
								cout<<"Invalid option. Press any key to try again.";
								_getch();
						}
								
					}while(walletOpt!=5);
					return;
			}
			else{
				cout <<"Incorrect PIN.\n";
				attempt--;
			}
			if(attempt==0){
				cout<<"Access Denied. Exiting Program.";
			}
		}while(attempt>0);
	} 
	else if (accountNum!=9215213192) {
		cout<<"Account number not registered. Press any key to continue...";
		_getch();
	}
}


void CashIn(){
	system("cls");
	cout << "Welcome to ePESO!\n[CASH IN]\n";
	if (activeAccount == 1)
    	cout << "Wallet Amount: PHP " << fixed << setprecision(2) << PresetWallet;
	else
    	cout << "Wallet Amount: PHP " << fixed << setprecision(2) << RegisteredWallet;
	cout << "\nEnter amount to cash in: ";
	cin >> cash;
	if (cash <=50000 && cash >0){
		cout<<"Press any key to confirm cash in...";
		_getch();
		if (activeAccount == 1)
    		PresetWallet += cash;
		else
   			RegisteredWallet += cash;
	} else {
		cout<<"Invalid amount. Please press any key to try again.";
		_getch();
	}
}



void CashOut(){
	system("cls");
	cout << "Welcome to ePESO!\n\n[CASH OUT]\n";
	if (activeAccount == 1)
    	cout << "Wallet Amount: PHP " << fixed << setprecision(2) << PresetWallet;
	else
    	cout << "Wallet Amount: PHP " << fixed << setprecision(2) << RegisteredWallet;
	cout << "\nEnter amount to cash out: ";
	cin >> cash;
	
	double balance;
	if (activeAccount == 1)
    	balance = PresetWallet;
	else
    	balance = RegisteredWallet;
		
	if (cash > 0 && cash <= balance) {
		cout<<"Press any key to confirm cash out...";
		_getch();
    	if (activeAccount == 1)
        	PresetWallet -= cash;
    	else
        	RegisteredWallet -= cash;
        	
	} else {
		cout<<"Invalid amount. Please press any key to try again.";
		_getch();
	}
}

//MY PART (PAY BILLS & BUY LOAD)
void PayBills() {
    int billerChoice;
    string biller;
    string billingNumber;
    double amount;
    char confirm;

    cout << "\n\n";
    cout << "Welcome to ePESO!\n[BILLS PAYMENT]\n\n";
    cout << "[1] Electricity\n[2] Water\n[3] Internet\n";
    cout << "Choose a biller: ";
    cin >> billerChoice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice. Press any key to continue...";
        _getch();
        return;
    }

    switch (billerChoice) {
        case 1: biller = "Electricity"; break;
        case 2: biller = "Water"; break;
        case 3: biller = "Internet"; break;
        default:
            cout << "Invalid choice. Press any key to continue...";
            _getch();
            return;
    }

    cout << "Enter fictional billing number: ";
    cin >> billingNumber;
    cout << "Enter payment amount: PHP ";
    cin >> amount;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid amount. Press any key to continue...";
        _getch();
        return;
    }
    if (amount <= 0) {
        cout << "Amount must be greater than zero. Press any key to continue...";
        _getch();
        return;
    }
    if ((activeAccount == 1 && amount > PresetWallet) ||
        (activeAccount == 0 && amount > RegisteredWallet)) {
        cout << "Insufficient balance. Press any key to continue...";
        _getch();
        return;
    }

    cout << "Pay PHP " << fixed << setprecision(2) << amount
         << " to " << biller << "? (Y/N): ";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Payment cancelled. Press any key to continue...";
        _getch();
        return;
    }

    if (activeAccount == 1)
        PresetWallet -= amount;
    else
        RegisteredWallet -= amount;

    cout << "\n=== PAYMENT RECEIPT ===\n";
    cout << "Biller: " << biller << '\n';
    cout << "Billing number: " << billingNumber << '\n';
    cout << "Amount paid: PHP " << amount << '\n';
    if (activeAccount == 1)
        cout << "Remaining balance: PHP " << PresetWallet << '\n';
    else
        cout << "Remaining balance: PHP " << RegisteredWallet << '\n';

//input Transaction history dito
    cout << "Press any key to continue...";
    _getch();
}

void BuyLoad() {
    int networkChoice;
    int loadChoice;
    int loadAmount;
    string network;
    string mobileNumber;
    char confirm;

    cout << "\n\n";
    cout << "Welcome to ePESO!\n[BUY MOBILE LOAD]\n\n";
    cout << "[1] Globe\n[2] Smart\n[3] DITO\n";
    cout << "Choose a network: ";
    cin >> networkChoice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice. Press any key to continue...";
        _getch();
        return;
    }

    switch (networkChoice) {
        case 1: network = "Globe"; break;
        case 2: network = "Smart"; break;
        case 3: network = "DITO"; break;
        default:
            cout << "Invalid choice. Press any key to continue...";
            _getch();
            return;
    }

    cout << "Enter recipient mobile number: ";
    cin >> mobileNumber;
    if (mobileNumber.length() != 11 || mobileNumber[0] != '0' ||
        mobileNumber[1] != '9') {
        cout << "Enter an 11-digit number starting with 09. Press any key...";
        _getch();
        return;
    }
    for (int i = 0; i < 11; i++) {
        if (mobileNumber[i] < '0' || mobileNumber[i] > '9') {
            cout << "Invalid mobile number. Press any key to continue...";
            _getch();
            return;
        }
    }

    cout << "[1] PHP 50\n[2] PHP 100\n[3] PHP 200\n";
    cout << "Choose load amount: ";
    cin >> loadChoice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice. Press any key to continue...";
        _getch();
        return;
    }

    switch (loadChoice) {
        case 1: loadAmount = 50; break;
        case 2: loadAmount = 100; break;
        case 3: loadAmount = 200; break;
        default:
            cout << "Invalid choice. Press any key to continue...";
            _getch();
            return;
    }

    if ((activeAccount == 1 && loadAmount > PresetWallet) ||
        (activeAccount == 0 && loadAmount > RegisteredWallet)) {
        cout << "Insufficient balance. Press any key to continue...";
        _getch();
        return;
    }

    cout << "Buy PHP " << loadAmount << " " << network
         << " load for " << mobileNumber << "? (Y/N): ";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Purchase cancelled. Press any key to continue...";
        _getch();
        return;
    }

    if (activeAccount == 1)
        PresetWallet -= loadAmount;
    else
        RegisteredWallet -= loadAmount;

    cout << "\n=== LOAD RECEIPT ===\n";
    cout << "Network: " << network << '\n';
    cout << "Mobile number: " << mobileNumber << '\n';
    cout << "Load purchased: PHP " << fixed << setprecision(2) << loadAmount * 1.0 << '\n';
    if (activeAccount == 1)
        cout << "Remaining balance: PHP " << PresetWallet << '\n';
    else
        cout << "Remaining balance: PHP " << RegisteredWallet << '\n';

    //Transaction history ulet dito
    cout << "Press any key to continue...";
    _getch();
}
