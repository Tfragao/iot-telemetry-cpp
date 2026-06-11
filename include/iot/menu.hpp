#pragma once

namespace iot::menu {
    enum class MenuChoice {
        ReadOnce,
        ReadMultiple,
        ShowAverage,
        Exit,
        Invalid
    };

    void print_menu();
    MenuChoice read_choice();
}