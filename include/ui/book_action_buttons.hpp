#pragma once

#include <fundamental/book.hpp>
#include <util/events.hpp>

#include <ftxui/component/component.hpp>

#include <functional>
#include <string>

class BookCartMembershipButtonBase : public ftxui::ComponentBase,
                                     public EventSubscriber {
public:
  BookCartMembershipButtonBase(const Book &book) : mBook(book), mButtonText() {
    using namespace ftxui;
    updateButtonText();
    subscribe(
        "updateCart",
        std::bind(&BookCartMembershipButtonBase::updateButtonText, this)
    );
    Add(Button(
        &mButtonText, std::bind(&BookCartMembershipButtonBase::action, this)
    ));
  }

  void updateButtonText() {
    mButtonText =
        Cart::getInstance().hasBook(mBook) ? "Remove from Cart" : "Add to Cart";
  }

  void action() {
    auto &cart = Cart::getInstance();
    if (cart.hasBook(mBook)) {
      cart.removeBook(mBook);
    } else {
      cart.addBook(mBook);
    }
    EventPublisher::getInstance().publish("updateCart", "");
  }

  bool Focusable() const {
    return true;
  }

private:
  Book mBook;
  std::string mButtonText;
};

ftxui::Component BookCartMembershipButton(const Book &book) {
  return ftxui::Make<BookCartMembershipButtonBase>(book);
}

using BookActionButtonFactory = ftxui::Component (*)(const Book &);
