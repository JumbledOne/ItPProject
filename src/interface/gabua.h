#pragma once

#include <iostream>
#include <string>

#include <session.h>
#include <transfer.h>
#include <interface_utils.h>
#include <tan.h>

namespace Gabua {
    using namespace std;

    inline void SendMoney(Session& session) {
        session.RequestScreen(SCREENS::WALLET);
        Account* sender = session.GetActiveAccount();
        if (!sender) {
            return;
        }
        InterfaceUtils::ClearScreen();
        cout << "Welcome to ePESO!\n[SEND MONEY]\n";
        cout << "Available balance: " << InterfaceUtils::FloatToPeso(sender->GetMoney());
        cout << "\nRecipient mobile number: ";
        string phone;
        cin >> phone;
        int recipient_id = Tan::FindPhone(session, phone);
        if (recipient_id < 0 || recipient_id == sender->GetId()) {
            cout << "Recipient not found or self-transfer.";
            Tan::Pause();
            return;
        }
        cout << "Recipient: " << session.database->GetAccount(recipient_id).name << "\n";
        cout << "Amount: ";
        float amount;
        if (!InterfaceUtils::ReadAmount(amount) || !sender->HasSufficientBalance(amount)) {
            cout << "Invalid amount or insufficient funds.";
            Tan::Pause();
            return;
        }
        char confirm;
        cout << "Confirm transfer? (Y/N): ";
        cin >> confirm;
        if (confirm != 'Y' && confirm != 'y') {
            cout << "Cancelled.\n";
            Tan::Pause();
            return;
        }
        if (TransferMoney(*session.database, sender->GetId(), recipient_id, amount)) {
            cout << "Transfer successful!";
            cout << "\nReference Number: " << InterfaceUtils::FormatReferenceNumber(session.database->GetLastTransactionId());
            cout << "\nRemaining balance: " << InterfaceUtils::FloatToPeso(sender->GetMoney()) << "\n";
        } else {
            cout << "Transfer failed.\n";
        }
        Tan::Pause();
    }
}
