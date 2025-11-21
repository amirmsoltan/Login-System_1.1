#ifndef CONSOLE_COLOR_H
#define CONSOLE_COLOR_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

enum ConsoleTextColor {
    DEFAULT = 0,
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
};

struct SetColor {
    ConsoleTextColor c;
    SetColor(ConsoleTextColor color) : c(color) {}
};

#ifdef _WIN32
inline void applyColor(ConsoleTextColor c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD winColor = 0;
    switch (c) {
    case BLACK:   winColor = 0; break;
    case BLUE:    winColor = 1; break;
    case GREEN:   winColor = 2; break;
    case CYAN:    winColor = 3; break;
    case RED:     winColor = 4; break;
    case MAGENTA: winColor = 5; break;
    case YELLOW:  winColor = 6; break;
    case WHITE:   winColor = 7; break;
    default:      winColor = 7; break;
    }
    SetConsoleTextAttribute(hConsole, winColor);
}
#else
inline void applyColor(ConsoleTextColor c) {
    switch (c) {
    case BLACK:   std::cout << "\033[30m"; break;
    case RED:     std::cout << "\033[31m"; break;
    case GREEN:   std::cout << "\033[32m"; break;
    case YELLOW:  std::cout << "\033[33m"; break;
    case BLUE:    std::cout << "\033[34m"; break;
    case MAGENTA: std::cout << "\033[35m"; break;
    case CYAN:    std::cout << "\033[36m"; break;
    case WHITE:   std::cout << "\033[37m"; break;
    default:      std::cout << "\033[0m"; break;
    }
}
#endif

inline std::ostream& operator<<(std::ostream& os, SetColor sc) {
    applyColor(sc.c);
    return os;
}

inline SetColor color(ConsoleTextColor c) {
    return SetColor(c);
}

// endlc: ??? endl ??? ??? ?? ?? ???? ??????
inline std::ostream& endlc(std::ostream& os) {
    os << std::endl;
    applyColor(DEFAULT);
    return os;
}

// reset: ??? ?? ???? ???? ?? ???? ??????
inline std::ostream& reset(std::ostream& os) {
    applyColor(DEFAULT);
    return os;
}

#endif