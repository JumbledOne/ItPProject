#pragma once
// Note that this header file exists because including the cpp file directly leads to the project not compiling properly for some reason

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <conio.h>
#include <optional>

#include <session.h>

using namespace std;

void DisplayMainScreen(Session& session);
void Login(Session& session);
void Register(Session& session);
void CashIn(Session& session);
void CashOut(Session& session);
