#pragma once

#include <fundamental/book.hpp>
#include <ui/book_action_buttons.hpp>
#include <ui/text_observer.hpp>

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <span>
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

class ComponentTableBase : public ftxui::ComponentBase {
public:
  ComponentTableBase(
      std::shared_ptr<std::vector<int>> columnWidths, bool resizable = false
  )
      : mColumnWidths(columnWidths),
        mResizable(resizable),
        mContainer(ftxui::Container::Vertical({})) {
    Add(mContainer);
  }

  void setContent(std::span<std::span<const ftxui::Component>> componentsGrid) {
    mContainer->DetachAllChildren();
    for (auto componentsRow : componentsGrid) {
      addRow(componentsRow);
    }
  }

  void addRow(std::span<const ftxui::Component> components) {
    auto row = generateRow(components);
    mContainer->Add(row);
  }

  std::shared_ptr<std::vector<int>> getColumnWidthsRef() {
    return mColumnWidths;
  }

  ftxui::Element Render() {
    using namespace ftxui;
    return mContainer->Render() | vscroll_indicator | yframe;
  }

  bool Focusable() const override {
    return true;
  }

private:
  std::shared_ptr<std::vector<int>> mColumnWidths;
  bool mResizable;
  ftxui::Component mContainer;

  constexpr static const int sMaxInitialColumnWidth = 32;

  void computeColumnWidths(std::span<const ftxui::Component> columns) {
    for (int columnIndex = 0; columnIndex < columns.size(); ++columnIndex) {
      if (columnIndex >= mColumnWidths->size()) {
        mColumnWidths->emplace_back();
      }
      auto tempElement = columns[columnIndex]->Render();
      tempElement->ComputeRequirement();
      auto requiredWidth = tempElement->requirement().min_x + 1;
      auto cappedWidth = std::min(requiredWidth, sMaxInitialColumnWidth);
      (*mColumnWidths)[columnIndex] =
          std::max(cappedWidth, (*mColumnWidths)[columnIndex]);
    }
  }

  ftxui::Component generateRow(std::span<const ftxui::Component> columns) {
    using namespace ftxui;

    if (mResizable) {
      return generateResizableRow(columns);
    }

    computeColumnWidths(columns);

    auto row = Container::Horizontal({});

    for (auto columnIter = columns.begin(); columnIter != columns.end();
         ++columnIter) {
      auto component = *columnIter;
      if (columnIter == columns.begin()) {
        component |= Renderer(flex);
      } else {
        auto columnIndex = columnIter - columns.begin();
        component = Renderer(component, [component, columnIndex, this] {
          return component->Render() |
                 size(WIDTH, EQUAL, (*mColumnWidths)[columnIndex]);
        });
      }
      row->Add(component);
      if (std::next(columnIter) != columns.end())
        row->Add(Renderer([] { return separator(); }));
    }

    return row;
  }

  ftxui::Component generateResizableRow(
      std::span<const ftxui::Component> columns, int columnIndex = 0
  ) {
    using namespace ftxui;
    if (columnIndex == columns.size() - 1) {
      // base case
      return columns[columnIndex];
    }
    if (columnIndex >= mColumnWidths->size()) {
      mColumnWidths->emplace_back();
    }
    auto tempElement = columns[columnIndex]->Render();
    tempElement->ComputeRequirement();
    auto requiredWidth = tempElement->requirement().min_x + 1;
    auto cappedWidth = std::min(requiredWidth, sMaxInitialColumnWidth);
    (*mColumnWidths)[columnIndex] =
        std::max(cappedWidth, (*mColumnWidths)[columnIndex]);
    return ResizableSplitLeft(
        columns[columnIndex], generateResizableRow(columns, columnIndex + 1),
        &(*mColumnWidths)[columnIndex]
    );
  }
};

std::shared_ptr<ComponentTableBase> ComponentTable(
    std::shared_ptr<std::vector<int>> columnWidths, bool resizable = false
) {
  return std::make_shared<ComponentTableBase>(columnWidths, resizable);
}

std::shared_ptr<ComponentTableBase> ComponentTable(bool resizable = false) {
  auto widths = std::make_shared<std::vector<int>>();
  widths->reserve(32);
  return std::make_shared<ComponentTableBase>(widths, resizable);
}

std::shared_ptr<ComponentTableBase> ComponentTable(
    std::span<std::span<const ftxui::Component>> componentsGrid,
    bool resizable = false
) {
  auto table = ComponentTable(resizable);
  table->setContent(componentsGrid);
  return table;
}

ftxui::Component BookTable(
    std::span<Book> books, std::shared_ptr<std::vector<int>> columnWidths,
    std::span<const BookActionButtonFactory> actionButtonFactories = {}
) {
  using namespace ftxui;
  columnWidths->reserve(32);
  auto tableHeader = ComponentTable(columnWidths, true);
  auto headerDummyBook =
      Book("", "Title", "Author", "Location", "ISBN", "Genre");
  auto tableHeaderComponents = std::vector{
      TextObserver(headerDummyBook, &Book::getTitle, true),
      TextObserver(headerDummyBook, &Book::getAuthor, true),
      TextObserver(headerDummyBook, &Book::getGenre, true),
      TextObserver(headerDummyBook, &Book::getLocation, true),
      TextObserver(headerDummyBook, &Book::getIsbn, true),
  };
  if (!actionButtonFactories.empty()) {
    tableHeaderComponents.emplace_back(Renderer([] {
      return text(" Actions ") | bold;
    }));
  }
  tableHeader->addRow(tableHeaderComponents);
  auto tableBody = ComponentTable(columnWidths, true);
  for (auto &book : books) {
    auto bookRowComponents = std::vector{
        TextObserver(book, &Book::getTitle),
        TextObserver(book, &Book::getAuthor),
        TextObserver(book, &Book::getGenre),
        TextObserver(book, &Book::getLocation),
        TextObserver(book, &Book::getIsbn),
    };
    if (!actionButtonFactories.empty()) {
      auto actionButtonContainer = Container::Horizontal({});
      for (auto actionButtonFactory : actionButtonFactories) {
        actionButtonContainer->Add(actionButtonFactory(book));
      }
      bookRowComponents.emplace_back(Renderer(actionButtonContainer, [=] {
        return vbox({
            actionButtonContainer->Render(),
        });
      }));
    }
    tableBody->addRow(bookRowComponents);
  }
  bool noResults = books.empty();
  return Renderer(Container::Vertical({tableHeader, tableBody}), [=] {
    return vbox({
               tableHeader->Render(),
               separator(),
               noResults ? text("No results.") | bold
                         : tableBody->Render() | flex,
           }) |
           flex;
  });
}

ftxui::Component BookTable(
    std::span<Book> books,
    std::span<const BookActionButtonFactory> actionButtonFactories = {}
) {
  return BookTable(
      books, std::make_shared<std::vector<int>>(), actionButtonFactories
  );
}
