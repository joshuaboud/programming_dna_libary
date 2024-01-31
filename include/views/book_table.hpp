#pragma once

#include <fundamental/book.hpp>

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <span>
#include <string>

#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/table.hpp"
#include "ftxui/util/ref.hpp" // for Ref

// ftxui::Component TableRow(ftxui::Component component) {
//   return std::move(component);
// }

// template <typename... Components>
// ftxui::Component
// TableRow(ftxui::Component component, int *width, Components... rest) {
//   using namespace ftxui;
//   return ResizableSplitLeft(
//       std::move(component), TableRow(std::forward<Components>(rest)...),
//       width
//   );
// }

// class ComponentTableRowBase : public ftxui::ComponentBase {
// public:
//   ComponentTableRowBase(std::shared_ptr<std::vector<int>> columnWidthsRef)
//       : mColumnWidths(columnWidthsRef) {}

//   void setContent(std::span<const ftxui::Component> components) {
//     DetachAllChildren();
//     Add(generateRow(components));
//   }

// private:
//   std::shared_ptr<std::vector<int>> mColumnWidths;

//   constexpr static const int sMaxInitialColumnWidth = 15;

//   ftxui::Component
//   generateRow(std::span<const ftxui::Component> columns, int columnIndex = 0)
//   {
//     using namespace ftxui;
//     if (columnIndex == columns.size() - 1) {
//       // base case
//       return columns[columnIndex];
//     }
//     if (columnIndex >= mColumnWidths->size()) {
//       mColumnWidths->emplace_back();
//     }
//     auto tempElement = columns[columnIndex]->Render();
//     tempElement->ComputeRequirement();
//     auto requiredWidth = tempElement->requirement().min_x;
//     auto cappedWidth = std::min(requiredWidth, sMaxInitialColumnWidth);
//     (*mColumnWidths)[columnIndex] =
//         std::max(cappedWidth, (*mColumnWidths)[columnIndex]);
//     return ResizableSplitLeft(
//         columns[columnIndex], generateRow(columns, columnIndex + 1),
//         &(*mColumnWidths)[columnIndex]
//     );
//   }
// };

// std::shared_ptr<ComponentTableRowBase>
// ComponentTableRow(std::shared_ptr<std::vector<int>> columnWidthsRef) {
//   return std::make_shared<ComponentTableRowBase>(columnWidthsRef);
// }

// std::shared_ptr<ComponentTableRowBase> ComponentTableRow(
//     std::span<const ftxui::Component> components,
//     std::shared_ptr<std::vector<int>> columnWidthsRef
// ) {
//   auto row = ComponentTableRow(columnWidthsRef);
//   row->setContent(components);
//   return row;
// }

class ComponentTableBase : public ftxui::ComponentBase {
public:
  ComponentTableBase(std::shared_ptr<std::vector<int>> columnWidths)
      : mColumnWidths(columnWidths),
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

private:
  std::shared_ptr<std::vector<int>> mColumnWidths;

  ftxui::Component mContainer;

  constexpr static const int sMaxInitialColumnWidth = 15;

  ftxui::Component
  generateRow(std::span<const ftxui::Component> columns, int columnIndex = 0) {
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
    auto requiredWidth = tempElement->requirement().min_x;
    auto cappedWidth = std::min(requiredWidth, sMaxInitialColumnWidth);
    (*mColumnWidths)[columnIndex] =
        std::max(cappedWidth, (*mColumnWidths)[columnIndex]);
    return ResizableSplitLeft(
        columns[columnIndex], generateRow(columns, columnIndex + 1),
        &(*mColumnWidths)[columnIndex]
    );
  }
};

std::shared_ptr<ComponentTableBase>
ComponentTable(std::shared_ptr<std::vector<int>> columnWidths) {
  return std::make_shared<ComponentTableBase>(columnWidths);
}

std::shared_ptr<ComponentTableBase> ComponentTable() {
  auto widths = std::make_shared<std::vector<int>>();
  widths->reserve(32);
  return std::make_shared<ComponentTableBase>(widths);
}

std::shared_ptr<ComponentTableBase>
ComponentTable(std::span<std::span<const ftxui::Component>> componentsGrid) {
  auto table = ComponentTable();
  table->setContent(componentsGrid);
  return table;
}

template <typename T>
using TextObserverMethod = const std::string &(T::*)() const;

template <typename T>
ftxui::Component TextObserver(
    const T &obj, TextObserverMethod<T> method, bool printBold = false
) {
  using namespace ftxui;
  return Renderer([obj, method, printBold] {
    auto e = paragraph(" " + (obj.*method)() + " ");
    return printBold ? (e | bold) : e;
  });
}

ftxui::Component BookTable(
    std::span<Book> books,
    std::span<const std::function<void(Book)>> actions = {}
) {
  using namespace ftxui;
  auto columnWidths = std::make_shared<std::vector<int>>();
  columnWidths->reserve(32);
  auto tableHeader = ComponentTable(columnWidths);
  auto headerDummyBook =
      Book("", "Title", "Author", "Location", "ISBN", "Genre");
  auto tableHeaderComponents = std::vector{
      TextObserver(headerDummyBook, &Book::getTitle, true),
      TextObserver(headerDummyBook, &Book::getAuthor, true),
      TextObserver(headerDummyBook, &Book::getGenre, true),
      TextObserver(headerDummyBook, &Book::getLocation, true),
      TextObserver(headerDummyBook, &Book::getIsbn, true),
  };
  if (!actions.empty()) {
    tableHeaderComponents.emplace_back(Renderer([] {
      return text(" Actions ") | bold;
    }));
  }
  tableHeader->addRow(tableHeaderComponents);
  auto tableBody = ComponentTable(columnWidths);
  for (auto &book : books) {
    auto bookRowComponents = std::vector{
        TextObserver(book, &Book::getTitle),
        TextObserver(book, &Book::getAuthor),
        TextObserver(book, &Book::getGenre),
        TextObserver(book, &Book::getLocation),
        TextObserver(book, &Book::getIsbn),
    };
    if (!actions.empty()) {
      auto actionButtonContainer = Container::Horizontal({});
      for (auto action : actions) {
        actionButtonContainer->Add(Button("Action", [book, action] {
          action(book);
        }));
      }
      bookRowComponents.emplace_back(Renderer(actionButtonContainer, [=] {
        return vbox({
            actionButtonContainer->Render(),
        });
      }));
    }
    tableBody->addRow(bookRowComponents);
  }
  return Renderer(Container::Vertical({tableHeader, tableBody}), [=] {
    return vbox({
               tableHeader->Render(),
               separator(),
               tableBody->Render() | vscroll_indicator | yframe | flex,
           }) |
           flex;
  });
}
