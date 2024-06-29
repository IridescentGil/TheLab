cmake_minimum_required(VERSION 3.5)

project(
  TheLab
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic "$<$<CONFIG:DEBUG>:-O0;-g3;>"
                      "$<$<CONFIG:RELEASE>:-O2;>")
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
  set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
      "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
      "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")
endif()

add_subdirectory(src)

option(UI "Build Qt6 UI" ON)
add_subdirectory(ui)

include(GNUInstallDirs)
install(
  TARGETS TheLab
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

enable_testing()
add_subdirectory(test)
