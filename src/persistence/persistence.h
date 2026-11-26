#pragma once
#include <fstream>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
#include <session.h>

namespace Persistence {
inline constexpr const char* FILE_NAME = "e_peso.txt";
inline bool Save(Session& session) {
    std::ofstream out(FILE_NAME, std::ios::trunc);
    if (!out) return false;
    out << "EPESO_V1\n";
    out << session.database->GetAccounts().size() << ' '
        << session.database->GetTransactions().size() << '\n';
    out << std::setprecision(9);
    // Account and transaction IDs are allocated consecutively in this simulation.
    for (int id = 0; id <= session.database->GetLastAccountId(); ++id) {
        if (!session.database->HasAccount(id)) continue;
        const auto& a = session.database->GetAccount(id);
        out << id << ' ' << std::quoted(a.name) << ' '
            << std::quoted(a.GetPhoneNumber()) << ' '
            << std::quoted(a.GetPin()) << ' ' << a.GetMoney() << '\n';
    }
    for (int id = 0; id <= session.database->GetLastTransactionId(); ++id) {
        if (!session.database->HasTransaction(id)) continue;
        const auto& t = session.database->GetTransaction(id);
        out << id << ' ' << t.from << ' ' << t.to << ' ' << t.amount << '\n';
    }
    return static_cast<bool>(out);
}
inline bool Load(Session& session) {
    std::ifstream in(FILE_NAME);
    if (!in) return false;
    std::string magic;
    size_t accountCount, transactionCount;
    if (!(in >> magic >> accountCount >> transactionCount) || magic != "EPESO_V1" ||
        accountCount > 10000 || transactionCount > 100000) return false;
    auto loaded = std::make_unique<Database>();
    for (size_t i = 0; i < accountCount; ++i) {
        int id; std::string name, phone, pin; float money;
        if (!(in >> id >> std::quoted(name) >> std::quoted(phone) >> std::quoted(pin) >> money) ||
            id != static_cast<int>(i) || !std::isfinite(money) || money < 0) return false;
        auto& a = loaded->CreateAccount();
        a.name = name;
        if (!a.SetPhoneNumber(phone) || !a.SetPin(pin)) return false;
        if (money > 0 && !a.AddMoney(money)) return false;
    }
    for (size_t i = 0; i < transactionCount; ++i) {
        int id, from, to; float amount;
        if (!(in >> id >> from >> to >> amount) || id != static_cast<int>(i) ||
            !loaded->HasAccount(from) || !loaded->HasAccount(to) ||
            !std::isfinite(amount) || amount <= 0) return false;
        loaded->CreateTransaction(from, to, amount);
    }
    session.database = std::move(loaded);
    return true;
}
} // namespace Persistence
