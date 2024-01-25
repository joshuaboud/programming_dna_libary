cmake_minimum_required(VERSION 3.21)

project(library_management_system
    VERSION 1
    DESCRIPTION "Library managment system"
    LANGUAGES CXX
)

add_compile_options(
  -Wall
  -Wextra
  -Werror=return-type
  -flto
  -fanalyzer
  -std=c++20
)

add_compile_definitions(
  $<$<CONFIG:Release,MinSizeRel,RelWithDebInfo>:NDEBUG>
)

add_link_options(
  -flto
)

include_directories(include)

add_executable(library_management_system
    src/main.cpp
)

add_subdirectory(lib/FTXUI)

target_include_directories(library_management_system PRIVATE lib/FTXUI/include)

target_link_libraries(library_management_system
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
