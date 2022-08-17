#include "terminal.h"

#include <iostream>

void rastrum::terminal::setColor(RGBA col) {
  std::cout << "\033[38;2;" << static_cast<int>(col.r) << ";" << static_cast<int>(col.g) << ";"
            << static_cast<int>(col.b) << "m";
}

void rastrum::terminal::clear() {
  std::cout << "\033[2J\033[1;1H";
}

void rastrum::terminal::reset() {
  std::cout << "\033[0;00m";
}