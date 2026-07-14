#include <iostream>
#include <iomanip>
#include <string>
#include "LoginModule.h"

using namespace std;

int main() {
    mainScreen();

    return 0;
}

void clearScreen() {
    cout << "\033[2J\033[H";
}