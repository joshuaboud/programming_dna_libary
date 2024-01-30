#pragma once

#include <management/cart.hpp>
#include <util/events.hpp>

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/table.hpp"
#include "ftxui/util/ref.hpp" // for Ref

class CartScreenBase : public ftxui::ComponentBase {
public:
  CartScreenBase() : ftxui::ComponentBase() {
    updateCartContents();
    EventPublisher::getInstance().subscribe("updateCart", [this] (const std::string &) {
      this->updateCartContents();
    });
  }

private:
  void updateCartContents() {
    using namespace ftxui;
    auto books = Cart::getInstance().getBooks();
    auto container = Container::Vertical({});
    for (auto &book : books) {
      auto removeBookButton = Button("Remove Book", [=] {
        Cart::getInstance().removeBook(book);
        EventPublisher::getInstance().publish("updateCart", "updateCart");
      });
      container->Add(Renderer(removeBookButton, [=] {
        return hbox({
                   paragraphAlignLeft(book.getTitle()) | flex,
                   separator(),
                   paragraphAlignLeft(book.getAuthor()) |
                       size(WIDTH, EQUAL, 20),
                   separator(),
                   paragraphAlignLeft(book.getGenre()) | size(WIDTH, EQUAL, 20),
                   separator(),
                   paragraphAlignLeft(book.getLocation()) |
                       size(WIDTH, EQUAL, 10),
                   separator(),
                   paragraphAlignLeft(book.getIsbn()) | size(WIDTH, EQUAL, 15),
                   separator(),
                   vbox({
                       removeBookButton->Render(),
                   }),
               }) |
               xflex;
      }));
    }
    DetachAllChildren();
    Add(container);
  }
};

ftxui::Component CartScreen() {
  return ftxui::Make<CartScreenBase>();
}
