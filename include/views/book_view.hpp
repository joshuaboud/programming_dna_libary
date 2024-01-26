#pragma once

#include <fundamental/book.hpp>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>

ftxui::Element BookDetailView(const Book &book) {
  using namespace ftxui;
  return window(
    text(" " + book.getTitle() + " ") | bold,
    vbox({
      hbox({
        text("Author: "), text(book.getAuthor())
      })
    })
  );
}
