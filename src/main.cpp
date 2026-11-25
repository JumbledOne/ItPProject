#include <iostream>
#include <memory>

#include <user.h>

std::unique_ptr<User> user;

int main() {
    // Initialize user state here
    user = std::make_unique<User>();

    // Display Screen?
    // TODO

    std::cout << "Input something: " << std::endl;
    int test;
    std::cin >> test;
    // Exit
    return 0;
}