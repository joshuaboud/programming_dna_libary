#pragma once

#include <interactive/book_filter.hpp>

#include <memory> // for allocator, __shared_ptr_access
#include <string> // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/table.hpp"
#include "ftxui/util/ref.hpp" // for Ref

inline auto InputWithLabel(
    ftxui::StringRef content, ftxui::StringRef placeholder,
    const std::string &label
) {
  using namespace ftxui;
  Component input = Input(content, placeholder);
  return Renderer(input, [=] {
    return hbox({
        text(" " + label + ": "),
        input->Render(),
    });
  });
}

auto SearchScreen(std::shared_ptr<Library> library) {
  using namespace ftxui;

  auto titleInput = std::make_shared<std::string>("");
  auto authorInput = std::make_shared<std::string>("");
  auto genreInput = std::make_shared<std::string>("");
  auto isbnInput = std::make_shared<std::string>("");

  auto results = std::make_shared<std::vector<Book>>();

  auto inputs = Container::Vertical({
      InputWithLabel(titleInput.get(), "Enter Book Title", "Title"),
      InputWithLabel(authorInput.get(), "Enter Book Author", "Author"),
      InputWithLabel(genreInput.get(), "Enter Book Genre", "Genre"),
      InputWithLabel(isbnInput.get(), "Enter ISBN", "ISBN"),
  });

  Component searchButton = Button("Search", [=] {
    results->clear();
    std::vector<BookFilter> filters;
    if (!titleInput->empty())
      filters.emplace_back(BookFilter::Key::TITLE, *titleInput);
    if (!authorInput->empty())
      filters.emplace_back(BookFilter::Key::AUTHOR, *authorInput);
    if (!genreInput->empty())
      filters.emplace_back(BookFilter::Key::GENRE, *genreInput);
    if (!isbnInput->empty())
      filters.emplace_back(BookFilter::Key::ISBN, *isbnInput);

    *results = library->fetchBooks(filters);
  });

  auto container = Container::Vertical({
      inputs,
      searchButton,
  });

  return Renderer(container, [=] {
    std::vector<std::vector<Element>> resultsVector = {
        {
         text("Title"),
         text("Author"),
         text("Genre"),
         text("Location"),
         text("ISBN"),
         },
    };
    for (Book &result : *results) {
      resultsVector.push_back({
          text(result.getTitle()),
          text(result.getAuthor()),
          text(result.getGenre()),
          text(result.getLocation()),
          text(result.getIsbn()),
      });
    }
    auto resultsTable = Table(resultsVector);
    resultsTable.SelectAll().SeparatorVertical(LIGHT);
    resultsTable.SelectRow(0).BorderBottom(LIGHT);
    resultsTable.SelectAll().Decorate(flex);
    return vbox({
        hbox({
            vbox({
                inputs->Render(),
            }) | flex,
            vbox({
                searchButton->Render() | center,
            }),
        }),
        separatorHeavy(),
        resultsTable.Render(),
    });
  });
}
