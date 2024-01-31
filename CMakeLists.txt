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
  -march=native
  $<$<CONFIG:Release,MinSizeRel,RelWithDebInfo>:-flto>
  $<$<CONFIG:Release,MinSizeRel,RelWithDebInfo>:-fanalyzer>
)

add_compile_definitions(
  $<$<CONFIG:Release,MinSizeRel,RelWithDebInfo>:NDEBUG>
)

add_link_options(
  -march=native
  $<$<CONFIG:Release,MinSizeRel,RelWithDebInfo>:-flto>
)

add_executable(library_management_system
  src/main.cpp
)

target_include_directories(library_management_system PRIVATE
  include
)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(lib/FTXUI)

target_include_directories(library_management_system PRIVATE lib/FTXUI/include)

target_link_libraries(library_management_system
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Check if OpenSSL is found
if(OPENSSL_FOUND)
  message(STATUS "OpenSSL found")
  message(STATUS "OpenSSL include directories: ${OPENSSL_INCLUDE_DIR}")
  message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
else()
  message(FATAL_ERROR "OpenSSL not found")
endif()

# Link against OpenSSL libraries
target_link_libraries(library_management_system PRIVATE ${OPENSSL_LIBRARIES})

# Include OpenSSL headers
target_include_directories(library_management_system PRIVATE ${OPENSSL_INCLUDE_DIR})

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(library_management_system PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(library_management_system PRIVATE lib/uuid_v4)
