#pragma once

#include <interactive/filter.hpp>
#include <interactive/librarian.hpp>
#include <management/cart.hpp>
#include <util/events.hpp>

#include <ui/book_action_buttons.hpp>
#include <ui/book_table.hpp>
#include <ui/book_view.hpp>

#include <functional>
#include <memory> // for allocator, __shared_ptr_access
#include <span>
#include <string> // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/table.hpp"
#include "ftxui/util/ref.hpp" // for Ref

class SearchScreenBase : public ftxui::ComponentBase {
public:
  SearchScreenBase()
      : ftxui::ComponentBase(),
        mTitleSearchStr(),
        mAuthorSearchStr(),
        mGenreSearchStr(),
        mIsbnSearchStr(),
        mLocationSearchStr(),
        mSearchResultsTable(ftxui::Container::Vertical({ftxui::Renderer([] {
          return ftxui::text("Press Search");
        })})),
        mColumnWidths(std::make_shared<std::vector<int>>()) {
    using namespace ftxui;

    InputOption inputOptions = InputOption::Default();
    inputOptions.multiline = false;
    inputOptions.on_enter = std::bind(&SearchScreenBase::onSearch, this);

    auto inputContainer = Container::Vertical({
        Input(&mTitleSearchStr, "Search by Title", inputOptions),
        Input(&mAuthorSearchStr, "Search by Author", inputOptions),
        Input(&mGenreSearchStr, "Search by Genre", inputOptions),
        Input(&mIsbnSearchStr, "Search by ISBN", inputOptions),
        Input(&mLocationSearchStr, "Search by Location", inputOptions),
    });

    Component searchButton =
        Button("Search", std::bind(&SearchScreenBase::onSearch, this));

    Add(Renderer(
        Container::Vertical({
            Container::Horizontal({
                inputContainer,
                searchButton,
            }),
            mSearchResultsTable,
        }),
        [=, this] {
          return vbox({
              hbox({
                  vbox({
                      text(" Title: "),
                      text(" Author: "),
                      text(" Genre: "),
                      text(" ISBN: "),
                      text(" Location: "),
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
              mSearchResultsTable->Render() | flex,
          });
        }
    ));
  }

private:
  void onSearch() {
    using namespace ftxui;

    auto filters =
        FiltersBuilder<Book>()
            .addFilterIfStringNotEmpty(mTitleSearchStr, &Book::getTitle)
            .addFilterIfStringNotEmpty(mAuthorSearchStr, &Book::getAuthor)
            .addFilterIfStringNotEmpty(mGenreSearchStr, &Book::getGenre)
            .addFilterIfStringNotEmpty(mIsbnSearchStr, &Book::getIsbn)
            .addFilterIfStringNotEmpty(mLocationSearchStr, &Book::getLocation)
            .get();

    auto results = Librarian::fetchBooks(filters);

    mSearchResultsTable->DetachAllChildren();
    mSearchResultsTable->Add(
        BookTable(results, mColumnWidths, {{&BookCartMembershipButton}})
    );
  }

  std::string mTitleSearchStr;
  std::string mAuthorSearchStr;
  std::string mGenreSearchStr;
  std::string mIsbnSearchStr;
  std::string mLocationSearchStr;

  ftxui::Component mSearchResultsTable;
  std::shared_ptr<std::vector<int>> mColumnWidths;
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
