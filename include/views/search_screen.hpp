#pragma once

#include <interactive/book_filter.hpp>

#include <functional>
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

class SearchScreenBase : public ftxui::ComponentBase {
public:
  SearchScreenBase()
      : ftxui::ComponentBase(),
        mTitleSearchStr(),
        mAuthorSearchStr(),
        mGenreSearchStr(),
        mIsbnSearchStr(),
        mResults() {
    using namespace ftxui;

    auto SearchOnEnter = CatchEvent([&](Event e) {
      if (e == Event::Character('\n')) {
        onSearch();
        return true;
      }
      return false;
    });

    auto inputContainer = Container::Vertical({
        Input(&mTitleSearchStr, "Search by Title") | SearchOnEnter,
        Input(&mAuthorSearchStr, "Search by Author") | SearchOnEnter,
        Input(&mGenreSearchStr, "Search by Genre") | SearchOnEnter,
        Input(&mIsbnSearchStr, "Search by ISBN") | SearchOnEnter,
    });

    Component searchButton =
        Button("Search", std::bind(&SearchScreenBase::onSearch, this));

    Add(Renderer(
        Container::Horizontal({
            inputContainer,
            searchButton,
        }),
        [=] {
          std::vector<std::vector<Element>> resultsVector = {
              {
                  text("Title"),
                  text("Author"),
                  text("Genre"),
                  text("Location"),
                  text("ISBN"),
              },
          };
          for (const Book &result : mResults) {
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
                      text(" Title: "),
                      text(" Author: "),
                      text(" Genre: "),
                      text(" ISBN: "),
                  }),
                  vbox({
                      inputContainer->Render(),
                  }) | flex,
                  separatorLight(),
                  vbox({
                      searchButton->Render() | center,
                  }),
              }),
              separatorHeavy(),
              resultsTable.Render() | flex_grow,
          });
        }
    ));
  }

  void onSearch() {
    mResults.clear();
    std::vector<BookFilter> filters;
    if (!mTitleSearchStr.empty())
      filters.emplace_back(BookFilter::Key::TITLE, mTitleSearchStr);
    if (!mAuthorSearchStr.empty())
      filters.emplace_back(BookFilter::Key::AUTHOR, mAuthorSearchStr);
    if (!mGenreSearchStr.empty())
      filters.emplace_back(BookFilter::Key::GENRE, mGenreSearchStr);
    if (!mIsbnSearchStr.empty())
      filters.emplace_back(BookFilter::Key::ISBN, mIsbnSearchStr);

    mResults = Library::getInstance().fetchBooks(filters);
  }

private:
  std::string mTitleSearchStr;
  std::string mAuthorSearchStr;
  std::string mGenreSearchStr;
  std::string mIsbnSearchStr;

  std::vector<Book> mResults;
};

ftxui::Component SearchScreen() {
  return ftxui::Make<SearchScreenBase>();
}

// auto SearchScreen(std::shared_ptr<Library> library) {
//   using namespace ftxui;

//   auto titleInput = std::make_shared<std::string>("");
//   auto authorInput = std::make_shared<std::string>("");
//   auto genreInput = std::make_shared<std::string>("");
//   auto isbnInput = std::make_shared<std::string>("");

//   auto results = std::make_shared<std::vector<Book>>();

//   auto inputs = Container::Vertical({
//       InputWithLabel(titleInput.get(), "Enter Book Title", "Title"),
//       InputWithLabel(authorInput.get(), "Enter Book Author", "Author"),
//       InputWithLabel(genreInput.get(), "Enter Book Genre", "Genre"),
//       InputWithLabel(isbnInput.get(), "Enter ISBN", "ISBN"),
//   });

//   Component searchButton = Button("Search", [=] {
//     results->clear();
//     std::vector<BookFilter> filters;
//     if (!titleInput->empty())
//       filters.emplace_back(BookFilter::Key::TITLE, *titleInput);
//     if (!authorInput->empty())
//       filters.emplace_back(BookFilter::Key::AUTHOR, *authorInput);
//     if (!genreInput->empty())
//       filters.emplace_back(BookFilter::Key::GENRE, *genreInput);
//     if (!isbnInput->empty())
//       filters.emplace_back(BookFilter::Key::ISBN, *isbnInput);

//     *results = library->fetchBooks(filters);
//   });

//   auto container = Container::Vertical({
//       inputs,
//       searchButton,
//   });

//   return Renderer(container, [=] {
//     std::vector<std::vector<Element>> resultsVector = {
//         {
//          text("Title"),
//          text("Author"),
//          text("Genre"),
//          text("Location"),
//          text("ISBN"),
//          },
//     };
//     for (Book &result : *results) {
//       resultsVector.push_back({
//           text(result.getTitle()),
//           text(result.getAuthor()),
//           text(result.getGenre()),
//           text(result.getLocation()),
//           text(result.getIsbn()),
//       });
//     }
//     auto resultsTable = Table(resultsVector);
//     resultsTable.SelectAll().SeparatorVertical(LIGHT);
//     resultsTable.SelectRow(0).BorderBottom(LIGHT);
//     resultsTable.SelectAll().Decorate(flex);
//     return vbox({
//         hbox({
//             vbox({
//                 inputs->Render(),
//             }) | flex,
//             vbox({
//                 searchButton->Render() | center,
//             }),
//         }),
//         separatorHeavy(),
//         resultsTable.Render(),
//     });
//   });
// }
