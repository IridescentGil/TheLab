cmake_minimum_required(VERSION 3.13)

project(
  TheLab
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(UI "Build Qt6 UI" ON)
option(BUILD_TESTING "Build tests" ON)
option(ENABLE_COVERAGE "Enable code coverage information" OFF)

add_subdirectory(src)

if(UI)
  add_subdirectory(ui)
endif()

include(GNUInstallDirs)
install(
  TARGETS TheLab Lab
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(BUILD_TESTING)
  find_program(MEMORYCHECK_COMMAND valgrind)

  set(VALGRIND_COMMAND_OPTIONS
      "-q --tool=memcheck --trace-children=yes --num-callers=51 --trace-children=yes --leak-check=full --track-origins=yes --gen-suppressions=all"
  )
  set(MEMORYCHECK_COMMAND_OPTIONS
      "-q --tool=memcheck --trace-children=yes --num-callers=51 --trace-children=yes --leak-check=full --track-origins=yes --gen-suppressions=all"
  )
  include(CTest)
  add_subdirectory(test)
endif()
