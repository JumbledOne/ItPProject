#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <database.h>

namespace Persistence {
    void Save(Database& database);
    void Load(Database& database);
}