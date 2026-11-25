#include <iostream>
#include <interface/menu.h>


int main() {
    // Create Menu
    Menu* menu = new Menu(16, 16);
    
    std::cout << "Input something: " << std::endl;
    int test;
    std::cin >> test;
    
    // Delete objects
    delete menu;
    return 0;
}