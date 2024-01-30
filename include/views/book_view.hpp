#pragma once

#include <fundamental/book.hpp>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>

ftxui::Element BookDetailView(const Book &book) {
  using namespace ftxui;
  return window(
      text(" " + book.getTitle() + " ") | bold,
      vbox({hbox({text("Author: "), text(book.getAuthor())})})
  );
}

ftxui::Element BookRowView(
    const Book &book, int mAuthorWidth = 20, int mGenreWidth = 20,
    int mLocationWidth = 10, int mIsbnWidth = 15
) {
  using namespace ftxui;
  return hbox({
             paragraphAlignLeft(book.getTitle()) | flex,
             separator(),
             paragraphAlignLeft(book.getAuthor()) |
                 size(WIDTH, EQUAL, mAuthorWidth),
             separator(),
             paragraphAlignLeft(book.getGenre()) |
                 size(WIDTH, EQUAL, mGenreWidth),
             separator(),
             paragraphAlignLeft(book.getLocation()) |
                 size(WIDTH, EQUAL, mLocationWidth),
             separator(),
             paragraphAlignLeft(book.getIsbn()) |
                 size(WIDTH, EQUAL, mIsbnWidth),
         }) |
         xflex;
}
