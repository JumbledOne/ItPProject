#pragma once

#include <screens.h>
#include <account.h>

// Pass this around to classes that need it rather than making it a singleton
class Session {
public:
    int account = 0;
    SCREENS screen = SCREENS::MAIN; // Placeholder value showing what screen th e user is in
};