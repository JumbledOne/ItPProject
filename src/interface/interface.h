#pragma once

#include <unordered_map>
#include <optional>
#include <functional>

#include <screens.h>
#include <session.h>
#include <tan.h>
#include <Ev.h>

namespace Interface {
    inline const std::unordered_map<SCREENS, std::function<void(Session&)>> SCREEN_TO_FUNCTION = {
        {SCREENS::MAIN, Tan::DisplayMainScreen},
        {SCREENS::REGISTRATION, Tan::Register},
        {SCREENS::LOGIN, Tan::Login},
        {SCREENS::CASH_IN, Tan::CashIn},
        {SCREENS::CASH_OUT, Tan::CashOut},
        {SCREENS::WALLET, Tan::Wallet},
        {SCREENS::PAY_BILLS, Ev::PayBills},
        {SCREENS::BUY_LOAD, Ev::BuyLoad}
    };

    // The main function that runs the interface on a loop
    // Runs until session.RequestExit() then main reads session.exit_code
    inline void ShowInterface(Session& session) {
        while (!session.exit_code) {
            SCREEN_TO_FUNCTION.at(session.screen)(session);
        }
    }
}
