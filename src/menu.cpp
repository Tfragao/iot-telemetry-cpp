#include "../include/iot/menu.hpp"
#include <iostream>
#include <limits>

namespace iot::menu {
    void print_menu() {
        std::cout << "========== IoT Telemetry Menu ==========\n";
        std::cout << "1. Read sensor once\n";
        std::cout << "2. Read sensor 5 times\n";
        std::cout << "3. Show average temperature\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose option: ";
    }

    MenuChoice read_choice() {
        int choice {};

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return MenuChoice::Invalid;
        }

        switch (choice)
        {
            case 1:
                return MenuChoice::ReadOnce;
            case 2:
                return MenuChoice::ReadMultiple;
            case 3:
                return MenuChoice::ShowAverage;
            case 0:
                return MenuChoice::Exit;
            default:
                return MenuChoice::Invalid;
        }
    }
}