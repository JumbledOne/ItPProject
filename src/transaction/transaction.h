#pragma once

#include <account.h>

struct Transaction {
    // Nullptr accounts are supposed to be displayed as "Unknown"
    Account* from = nullptr; 
    Account* to = nullptr;
    float amount;
};