#pragma once

#include <string>

#include <session.h>

namespace Parser {

    // Keeps track of what was read until it's time to make them into real objects
    class BaseParserProxy {
    protected:
        bool realized = false; // Whether Realize made the object

    public:
        virtual ~BaseParserProxy() = default;
        virtual void RunCommand(const std::string& command, const std::string& value) = 0; // Commands that aren't recognised are ignored
        // Makes the object even if some parts were corrupted (those fall back to defaults)
        // Only does nothing if what the create function needs is missing
        virtual void Realize(Session& session) = 0;
    };

    // The DATABASE block. Restores the last ids since the ones that were last handed out could have been deleted
    class DatabaseProxy : public BaseParserProxy {
    private:
        int last_account_id = -1;
        int last_transaction_id = -1;

    public:
        void RunCommand(const std::string& command, const std::string& value) override;
        void Realize(Session& session) override; // Realize the accounts and transactions first
    };

    class AccountProxy : public BaseParserProxy {
    private:
        int id = -1; // Account number
        std::string name;
        std::string phone_number; // Empty means there is none
        std::string pin; // Empty means there is none
        float money = 0;

    public:
        void RunCommand(const std::string& command, const std::string& value) override;
        void Realize(Session& session) override;
        void ApplyBalance(Session& session) const; // Must overwrite the balance since creating transactions sets it
    };

    class TransactionProxy : public BaseParserProxy {
    private:
        int id = -1; // Reference number
        int from = -1; // Account numbers. -1 means there is none (e.g. no sender for a cash in)
        int to = -1;
        float amount = 0;
        TRANSACTION_TYPE type = TRANSACTION_TYPE::TRANSFER;

    public:
        void RunCommand(const std::string& command, const std::string& value) override;
        void Realize(Session& session) override;
    };
}
