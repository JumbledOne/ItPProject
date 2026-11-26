#pragma once

#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

// _getch is only on some compilers/platforms
#if __has_include(<conio.h>)
#include <conio.h>
#define INTERFACE_UTILS_HAS_CONIO
#endif

namespace InterfaceUtils {

    using namespace std;

    constexpr int REFERENCE_NUMBER_LENGTH = 10;
    constexpr int REFERENCE_NUMBER_GROUPINGS = 5; // So that 1234567890 is instead 12345 67890

    const string OPTION_PROMPT = "Choose option: ";

    // Removes everything past the first two decimal points
    // A float can't hold most cents exactly (0.53 is really 0.52999997) so the tiny error is allowed for
    inline float TruncateCurrency(float value) {
        double cents = (double)value * 100.0;
        return (float)(trunc(cents * (1.0 + 1.2e-7)) / 100.0);
    }

    // The value rounded to the nearest cent
    // Use for values that are already money (balances, amounts) since adding floats leaves tiny errors
    inline float RoundCurrency(float value) {
        return (float)(round((double)value * 100.0) / 100.0);
    }

    // The number with two decimals and nothing else, e.g. 1234.5 -> "1234.50"
    // Use for anything that shows or saves currency so they all agree
    inline string FormatCurrency(float value) {
        if (!isfinite(value)) {
            return "0.00";
        }
        ostringstream output;
        output << fixed << setprecision(2) << RoundCurrency(value);
        return output.str();
    }

    // Use for displaying currency values
    inline string FloatToPeso(float value) {
        return "PHP " + FormatCurrency(value);
    }

    inline void ClearScreen() {
        system("cls");
    }

    // Resets cin's error state and throws away the rest of the line
    inline void ClearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Reads one value from what the user typed then throws away the rest of that line
    // Otherwise what is left (the "es" of "yes") would be taken as the answer to the next question
    // Returns false if it couldn't be read as a T
    template <typename T>
    inline bool ReadValue(T& value) {
        bool success = static_cast<bool>(cin >> value);
        ClearInput();
        return success;
    }

    // Reads the whole line that was typed, spaces included. Use for names
    inline void ReadLine(string& line) {
        getline(cin, line);
    }

    // Waits for the user to press a key
    // Uses _getch since it doesn't need Enter. If that isn't available or fails, it waits for Enter on cin instead
    inline void AwaitInput() {
#ifdef INTERFACE_UTILS_HAS_CONIO
        if (_getch() != EOF) {
            return;
        }
#endif
        string ignored;
        getline(cin, ignored);
    }

    // Reads a currency amount from a line. Returns false if it isn't a plain number above zero (digits and at most one point)
    // Digits past two decimal places are dropped from the text so there is no float error. "10.999" is 10.99
    // Anything else on the line makes it invalid so "1,5" isn't quietly taken as 1
    inline bool ReadAmount(float& amount) {
        string line;
        getline(cin, line);

        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start == string::npos) {
            return false;
        }
        string text = line.substr(start, end - start + 1);

        string whole, fraction;
        bool seen_point = false;
        for (char c : text) {
            if (c == '.' && !seen_point) {
                seen_point = true;
            } else if (c >= '0' && c <= '9') {
                (seen_point ? fraction : whole) += c;
            } else {
                return false;
            }
        }
        if (whole.empty() && fraction.empty()) {
            return false;
        }
        if (whole.size() > 9) {
            return false; // Far past anything that can be in an account, and a float can't hold the cents of more digits than this anyway
        }
        fraction = (fraction + "00").substr(0, 2);

        long long cents = (whole.empty() ? 0 : stoll(whole)) * 100 + stoll(fraction);
        amount = (float)(cents / 100.0);
        return cents > 0;
    }

    // Reads an account number. All the whitespace is removed first (so "00000 00001" is 1 as the list shows it)
    // then the whole string is converted to an int. Returns -1 if that isn't possible
    inline int ReadAccountNumber() {
        string line;
        getline(cin, line);

        string text;
        for (char c : line) {
            if (!isspace((unsigned char)c)) {
                text += c;
            }
        }
        if (text.empty()) {
            return -1;
        }

        char* end = nullptr;
        errno = 0;
        long number = strtol(text.c_str(), &end, 10);
        if (*end != '\0' || errno == ERANGE || number < numeric_limits<int>::min() || number > numeric_limits<int>::max()) {
            return -1; // Not all of it was a number, or it doesn't fit
        }
        return (int)number;
    }

    // Reads a whole line and removes the spaces and tabs around it. An empty result means the user just pressed Enter
    inline void ReadLineTrimmed(string& line) {
        getline(cin, line);
        size_t start = line.find_first_not_of(" \t");
        if (start == string::npos) {
            line = "";
            return;
        }
        size_t end = line.find_last_not_of(" \t");
        line = line.substr(start, end - start + 1);
    }

    // Pads with leading zeros and groups the digits, e.g. 42 -> "00000 00042"
    inline string FormatReferenceNumber(int reference_number) {
        string output;
        string reference_number_string = to_string(reference_number);
        int padding = REFERENCE_NUMBER_LENGTH - (int)reference_number_string.size();
        for (int i = 0; i < REFERENCE_NUMBER_LENGTH; i++) {
            // No space before the first one
            if (i > 0 && i % REFERENCE_NUMBER_GROUPINGS == 0) {
                output += " ";
            }
            // Leading zeros first, then the actual digits
            if (i < padding) {
                output += '0';
            } else {
                output += reference_number_string[i - padding];
            }
        }
        return output;
    }
}
