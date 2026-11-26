#pragma once

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
    // Use for inputting currency
    inline float TruncateCurrency(float value) {
        return trunc(value * 100.0f) / 100.0f;
    }

    // Use for displaying currency values
    inline string FloatToPeso(float value) {
        ostringstream output;
        output << "PHP " << fixed << setprecision(2) << TruncateCurrency(value);
        return output.str();
    }

    inline void ClearScreen() {
        system("cls");
    }

    // Resets cin's error state and throws away the rest of the line
    // Use after a failed read, or before getline to drop the Enter left over from a previous cin >>
    inline void ClearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Waits for the user to press a key
    // Uses _getch since it doesn't need Enter. If that isn't available or fails, it waits for Enter on cin instead
    inline void AwaitInput() {
#ifdef INTERFACE_UTILS_HAS_CONIO
        if (_getch() != EOF) {
            return;
        }
#endif
        cin.clear();
        cin.ignore(1000, '\n'); // Whatever is left from a previous cin >>
        cin.get();
    }

    // Reads a currency amount. Returns false (and clears the input) if it isn't a number or isn't above zero
    // Anything past two decimal places is dropped
    inline bool ReadAmount(float& amount) {
        if (!(cin >> amount)) {
            ClearInput();
            return false;
        }
        amount = TruncateCurrency(amount);
        return isfinite(amount) && amount > 0;
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
