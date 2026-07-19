#pragma once

#ifndef LoginModule
#define LoginModule

#include <string>
using namespace std;

int exit();
int validateUser(string username, string password);
void loginScreen();
void registerScreen();

#endif