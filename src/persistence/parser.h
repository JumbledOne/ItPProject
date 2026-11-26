#pragma once

#include <array>
#include <cctype>
#include <exception>
#include <fstream>
#include <string>
#include <forward_list>

#include <session.h>
#include <parser_proxies.h>

namespace Parser {
    // Reads the whole file and handles the database by itself
    inline void ParseSaveFile(std::ifstream& file, Session& session) {
        // Reading values
        std::array<std::string, 2> command; // [0] is the command, [1] is the value
        bool is_reading_value = false; // False while still reading the command
        char character;
        bool is_end_of_file = false;

        // Storing data
        BaseParserProxy* current_proxy = nullptr; // Where the commands go. Null until the first object starts
        DatabaseProxy database_proxy;
        std::forward_list<AccountProxy> account_proxies;
        std::forward_list<TransactionProxy> transaction_proxies;

        // Now iterate through every character
        while (!is_end_of_file) {
            is_end_of_file = !file.get(character);
            if (is_end_of_file) {
                character = '\n'; // The last line might not end with a new line so this makes sure it runs
            }

            // Files with Windows line endings read the same
            if (character == '\r') {
                continue;
            }
            // New lines run the commands
            if (character == '\n') {
                // Check if the command creates a proxy first
                if (command[0] == "DATABASE") {
                    current_proxy = &database_proxy;
                } else if (command[0] == "ACCOUNT") {
                    account_proxies.emplace_front();
                    current_proxy = &account_proxies.front();
                } else if (command[0] == "TRANSACTION") {
                    transaction_proxies.emplace_front();
                    current_proxy = &transaction_proxies.front();
                } else if (command[0] != "" && current_proxy) {
                    try {
                        current_proxy->RunCommand(command[0], command[1]);
                    } catch (const std::exception&) {
                        // stoi and stof throw when a value isn't a number. That property just keeps its default
                    }
                }

                // Clear command for the next line
                command[0] = "";
                command[1] = "";
                is_reading_value = false;
            }
            // Everything after the command is the value, including spaces, so names can have them
            else if (is_reading_value) {
                command[1] += character;
            }
            // The first space after the command starts the value. Spaces and tabs before it are just indentation
            else if (std::isspace((unsigned char)character)) {
                if (command[0] != "") {
                    is_reading_value = true;
                }
            }
            else {
                command[0] += character;
            }
        }

        // Now make the real objects. The accounts have to exist before the transactions use them
        for (AccountProxy& proxy : account_proxies) {
            proxy.Realize(session);
        }
        for (TransactionProxy& proxy : transaction_proxies) {
            proxy.Realize(session);
        }
        // Creating the transactions changed the balances so put the saved ones back
        for (const AccountProxy& proxy : account_proxies) {
            proxy.ApplyBalance(session);
        }
        database_proxy.Realize(session);
    }

}
