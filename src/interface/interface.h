#pragma once

#include <unordered_map>
#include <optional>
#include <functional>
#include <iostream>

#include <screens.h>
#include <session.h>

#include <tan.h>
#include <Ev.h>
#include <blones.h>
#include <gabua.h>

namespace Interface {
    inline const std::unordered_map<SCREENS, std::function<void(Session&)>> SCREEN_TO_FUNCTION = {
        {SCREENS::MAIN, Tan::DisplayMainScreen},
        {SCREENS::REGISTRATION, Tan::Register},
        {SCREENS::LOGIN, Tan::Login},
        {SCREENS::CASH_IN, Tan::CashIn},
        {SCREENS::CASH_OUT, Tan::CashOut},
        {SCREENS::WALLET, Tan::Wallet},
        {SCREENS::PAY_BILLS, Ev::PayBills},
        {SCREENS::BUY_LOAD, Ev::BuyLoad},
        {SCREENS::SEND_MONEY, Gabua::SendMoney},
        {SCREENS::VIEW_TRANSACTIONS, Blones::ViewTransactions},
        {SCREENS::CHANGE_NAME, Blones::ChangeName},
        {SCREENS::CHANGE_PIN, Blones::ChangePin},
        {SCREENS::DELETE_ACCOUNT, Blones::AccountDeletion}
    };

    // The main function that runs the interface on a loop
    // Runs until session.RequestExit() then main reads session.exit_code
    inline void ShowInterface(Session& session) {
        // If the input ends (Ctrl+Z or a closed pipe) every screen would retry the failed read forever
        // So reaching the end throws and it's turned into an exit, which lets main save
        std::cin.exceptions(std::ios::eofbit);
        while (!session.exit_code) {
            try {
                SCREEN_TO_FUNCTION.at(session.screen)(session);
            } catch (const std::ios_base::failure&) {
                session.RequestExit(1);
            }
        }
    }
}
