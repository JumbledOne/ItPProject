#pragma once
// Note that this header file exists because including the cpp file directly leads to the project not compiling properly for some reason

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <conio.h>
#include <optional>

#include <session.h>

using namespace std;

optional<int> DisplayMainScreen(Session& session);
void Login();
optional<int> Register(Session& session);
void CashIn();
void CashOut();