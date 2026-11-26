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

    // Add the money to the recipient first.
    if (!recipient.AddMoney(amount)) {
        return false;
    }

    // Deduct the amount from the sender.
    if (!sender.DeductMoney(amount)) {
        recipient.DeductMoney(amount);
        return false;
    }

    // Record the successful transaction.
    database.CreateTransaction(&sender, &recipient, amount, TRANSACTION_TYPE::TRANSFER);

    return true;
}