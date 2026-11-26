#pragma once

#include <session.h>
#include <iostream>

namespace Blones {
    
    using namespace std;

    void ViewTransactions(Session& session) {
        // Check first if the account even exists
        Account* account = session.active_account;
        if (!account) {
            return;
        }
        system("cls"); // Clear screen first
        
    }

    void ViewAccountInformation(Session& session) {
        // Check first if the account even exists
        Account* account = session.active_account;
        if (!account) {
            return;
        }
        system("cls"); // Clear screen first
        
    }
}