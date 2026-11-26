#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <database.h>

namespace Persistence {
    // Saves the database to disk
    inline void Save(Session& session) {
        std::ofstream file("e_peso.txt");

        if (!file.is_open()) {
            return;
        }
        file << "Hello World";
        file.close();
    }

    // Loads data onto the database class directly
    inline void Load(Session& session) {

    }
}
