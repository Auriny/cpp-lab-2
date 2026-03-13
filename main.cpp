#include <windows.h>
#include "Station.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
//    std::setlocale(LC_ALL, ".UTF8");

    Station station;
    station.StartGame();

    return 0;
}