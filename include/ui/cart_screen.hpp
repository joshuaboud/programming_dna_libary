#pragma once

#include <management/cart.hpp>
#include <ui/book_table.hpp>
#include <util/events.hpp>

#include <memory>
#include <string>
#include <tuple>

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/table.hpp"
#include "ftxui/util/ref.hpp" // for Ref

class CartScreenBase : public ftxui::ComponentBase, public SubscribesToEvents {
public:
  CartScreenBase()
      : ftxui::ComponentBase(),
        mColumnWidths(std::make_shared<std::vector<int>>()) {
    updateCartContents();
    subscribe("updateCart", [this](const std::string &) {
      this->updateCartContents();
    });
  }

private:
  std::shared_ptr<std::vector<int>> mColumnWidths;

  void updateCartContents() {
    using namespace ftxui;
    auto books = Cart::getInstance().getBooks();
    DetachAllChildren();
    Add(BookTable(books, mColumnWidths, {{&BookCartMembershipButton}}));
  }
};

ftxui::Component CartScreen() {
  return ftxui::Make<CartScreenBase>();
}
