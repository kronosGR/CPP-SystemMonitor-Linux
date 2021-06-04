#include "ncurses_display.h"
#include "system.h"

#include "linux_parser.h"
#include <iostream>

int main() {
  System system;
  NCursesDisplay::Display(system);
  std::cout << LinuxParser::UpTime(75508) << "\n";
}