#include <transfer.h>
#include <cmath>

// Transfers virtual money between two registered accounts.
// Returns true when the transaction is successful.
bool TransferMoney(Database& database,
                   int senderId,
                   int recipientId,
                   float amount) {

    // Check if the amount is valid.
    if (!std::isfinite(amount) || amount <= 0) {
        return false;
    }

    // Prevent users from sending money to themselves.
    if (senderId == recipientId) {
        return false;
    }

    // Check whether both accounts exist.
    if (!database.HasAccount(senderId) ||
        !database.HasAccount(recipientId)) {
        return false;
    }

    // Retrieve the sender and recipient accounts.
    Account& sender = database.GetAccount(senderId);
    Account& recipient = database.GetAccount(recipientId);

    // Check whether the sender has sufficient balance.
    if (!sender.HasSufficientBalance(amount)) {
        return false;
    }

    // Creating the transaction is what moves the money: it's deducted from the sender and added to the recipient.
    database.CreateTransaction(&sender, &recipient, amount, TRANSACTION_TYPE::TRANSFER);

    return true;
}