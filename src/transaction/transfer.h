#pragma once

#include <database.h>

// Transfers virtual money between two existing accounts.
// Returns true if the transfer succeeds.
bool TransferMoney(Database& database,
                   int senderId,
                   int recipientId,
                   float amount);