
#include <iostream>
#include <memory>

#include <fundamental/library.hpp>
#include <management/cart.hpp>

#include <ui/main_view.hpp>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/dom/table.hpp"

int main(int argc, char *argv[]) {

  auto screen = ftxui::ScreenInteractive::Fullscreen();

  screen.Loop(MainView());



 
  return 0;
}
