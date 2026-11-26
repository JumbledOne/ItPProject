#pragma once

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

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
