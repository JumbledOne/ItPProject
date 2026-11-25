#pragma once
// Note that this header file exists because including the cpp file directly leads to the project not compiling properly for some reason

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <conio.h>
#include <optional>

using namespace std;

optional<int> DisplayMainScreen();
void Login();
void Register();
void CashIn();
void CashOut();