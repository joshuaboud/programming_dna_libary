#pragma once

#include <views/search_screen.hpp>

#include <memory> // for allocator, __shared_ptr_access
#include <string> // for char_traits, operator+, string, basic_string
#include <vector>

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/table.hpp"
#include "ftxui/util/ref.hpp" // for Ref

auto MainView(std::shared_ptr<Library> library) {
  using namespace ftxui;

  static int tabIndex = 0;

  static std::vector<std::string> tabHeaders = {
      "Book Search",
      "Cart",
  };

  auto tabToggle = Toggle(&tabHeaders, &tabIndex);

  auto tabContainer = Container::Tab(
      {
          SearchScreen(library),
          Renderer([] { return text("TODO") | bold; }),
      },
      &tabIndex
  );

  auto container = Container::Vertical({
      tabToggle,
      tabContainer,
  });

  return Renderer(container, [=] {
    return vbox({
               hbox({
                   tabToggle->Render(),
                   separator(),
               }),
               separator(),
               tabContainer->Render(),
           }) |
           border;
  });
}
