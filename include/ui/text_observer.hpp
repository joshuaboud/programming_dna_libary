#pragma once

#include <ftxui/component/component.hpp>

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
