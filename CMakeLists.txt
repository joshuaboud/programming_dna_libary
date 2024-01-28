cmake_minimum_required(VERSION 3.21)

project(library_management_system
  VERSION 1
  DESCRIPTION "Library managment system"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  -Wall
  -Wextra
  -Werror=return-type
  # -flto
  # -fanalyzer
)

add_compile_definitions(
  $<$<CONFIG:Release,MinSizeRel,RelWithDebInfo>:NDEBUG>
)

add_link_options(
  # -flto
)

add_executable(library_management_system
  src/main.cpp
)

target_include_directories(library_management_system PRIVATE
  include
)

add_subdirectory(lib/FTXUI)

target_include_directories(library_management_system PRIVATE lib/FTXUI/include)

target_link_libraries(library_management_system
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
