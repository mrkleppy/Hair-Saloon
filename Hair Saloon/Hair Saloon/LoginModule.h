#pragma once

#ifndef Login
#define Login

#include <string>
using namespace std;

void clearScreen();
int exit();
int validateUser(string username, string password);
void loginScreen();
void registerScreen();
void mainScreen();

#endif