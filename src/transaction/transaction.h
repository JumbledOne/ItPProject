#pragma once

enum class TRANSACTION_TYPE {
    TRANSFER,
    CASH_IN,
    CASH_OUT,
    BILL_PAYMENT,
    LOAD_PURCHASE
};

class Account; // Forward declared since account.h includes this file

struct Transaction {
    int reference_number;
    // Not owned. Null if there is no such account (e.g. no sender for a cash in)
    Account* from = nullptr;
    Account* to = nullptr;
    float amount;
    TRANSACTION_TYPE type = TRANSACTION_TYPE::TRANSFER;
};