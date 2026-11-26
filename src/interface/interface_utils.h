#pragma once

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

namespace InterfaceUtils {

    using namespace std;

    // Removes everything past the first two decimal points
    // Use for inputting currency
    inline float TruncateCurrency(float value) {
        return trunc(value * 100.0f) / 100.0f;
    }

    // Use for displaying currency values
    inline string FloatToPeso(float value) {
        ostringstream output;
        output << "P " << fixed << setprecision(2) << TruncateCurrency(value);
        return output.str();
    }

    inline void ClearScreen() {
        system("cls");
    }
}
