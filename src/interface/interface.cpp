#include <interface.h>

std::optional<int> DisplayScreen() {
    std::optional<int> interface_result = std::nullopt;
    while (!(interface_result)) { // Waits until the interface result returns something
       interface_result = DisplayMainScreen(); 
    }
}