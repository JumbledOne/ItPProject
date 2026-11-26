#pragma once
// interface.h and session.h both use this
// This is a separate header to avoid a circular dependency

enum class SCREENS {
    MAIN,
    REGISTRATION,
    LOGIN,
    CASH_IN,
    CASH_OUT,
    WALLET,
    PAY_BILLS,
    BUY_LOAD,
    SEND_MONEY,
    VIEW_TRANSACTIONS,
    CHANGE_NAME,
    CHANGE_PIN,
    DELETE_ACCOUNT
};
