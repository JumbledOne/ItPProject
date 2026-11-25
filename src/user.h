#pragma once

#include <account.h>

// Pass this around to classes that need it rather than making it a singleton
class User {
public:
    Account& account;
    int screen; // Placeholder value showing what screen th e user is in
};