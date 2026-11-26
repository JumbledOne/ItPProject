#include <tan.h>

int opt, walletOpt, PIN, attempt, activeAccount;
long long accountNum, accNum, pw;
double cash, RegisteredWallet, PresetWallet=5000;

void DisplayMainScreen(Session& session){
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
			session.RequestScreen(SCREENS::REGISTRATION);
			break;
		case 3:
			cout << "Exiting program...";
			session.RequestExit(0);
			break;
		default:
			cout <<"Invalid option. Press any key to continue...";
			_getch();
			break;
	}
}
void Register(Session& session){
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
					cout <<"[3] Log Out\n";
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
							break;
						default:
							cout<<"Invalid option. Press any key to try again.";
							_getch();
							break;
					}
										
				}while(walletOpt!=3);
				session.RequestScreen(SCREENS::MAIN);
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
	session.RequestScreen(SCREENS::MAIN);
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
						cout <<"[3] Log Out\n";
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
								break;
							default:
								cout<<"Invalid option. Press any key to try again.";
								_getch();
						}
								
					}while(walletOpt!=3);
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
