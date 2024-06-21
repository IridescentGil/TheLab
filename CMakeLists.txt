cmake_minimum_required(VERSION 3.5)

project(
  TheLab
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(DOCUMENTATION ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(SQLiteCpp REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/excercise/excercise.cpp
    src/excercise/excercise.h
    src/body/body.h
    src/body/body.cpp
    src/database/database.h
    src/database/database.cpp
    src/history/history.cpp
    src/history/history.h
    src/analytics/analytics.h
    src/analytics/analytics.cpp
    src/workout/workout.h
    src/workout/workout.cpp
    src/lab/lab.h
    src/lab/lab.cpp)

include_directories(
  src
  src/excercise
  src/body
  src/database
  src/analytics
  src/history
  src/workout
  src/lab)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic "$<$<CONFIG:DEBUG>:-O0;-g3;>"
                      "$<$<CONFIG:RELEASE>:-O2;>")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(
    TheLab
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    src/mainwindow.ui
    src/mainwindow.h
    src/main.cpp
    src/mainwindow.cpp)
  # Define target properties for Android with Qt 6 as: set_property(TARGET
  # TheLab APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(TheLab SHARED ${PROJECT_SOURCES})
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(TheLab ${PROJECT_SOURCES})
  endif()
endif()

target_link_libraries(TheLab PRIVATE Qt${QT_VERSION_MAJOR}::Widgets SQLiteCpp
                                     sqlite3 pthread dl)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TheLab)
endif()
set_target_properties(
  TheLab
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS TheLab
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(TheLab)
endif()

if(DOCUMENTATION)
  find_package(Doxygen REQUIRED dot)
  if(DOXYGEN_FOUND)
    doxygen_add_docs(DOCS ${PROJECT_SOURCES} ALL)
  endif()
endif()

enable_testing()

if($ENV{GOOGLETEST_DIR})
  get_filename_component(_GTEST_BASE $ENV{GOOGLETEST_DIR} REALPATH)
elseif(EXISTS "") # set by QC
  get_filename_component(_GTEST_BASE "" REALPATH) # set by QC
endif()

if(NOT GTEST_ROOT)
  if(_GTEST_BASE)
    message("Setting GTEST_ROOT to ${_GTEST_BASE}")
    set(GTEST_ROOT ${_GTEST_BASE})
  else()
    message("No GTEST_ROOT specified - using system defaults.")
  endif()
endif()

find_package(GTest REQUIRED)
if(NOT GTest_FOUND)
  message(FATAL_ERROR "No GTest Found")
endif()

# add_test(NAME programTests COMMAND programTests)

add_executable(
  TheLabTest
  test/testHelper.cpp
  test/excercise_test.cpp
  src/excercise/excercise.cpp
  test/database_test.cpp
  src/database/database.cpp
  test/body_test.cpp
  src/body/body.cpp
  src/history/history.cpp
  test/history_test.cpp
  src/analytics/analytics.cpp
  test/analytics_test.cpp
  src/workout/workout.cpp
  test/workout_test.cpp
  src/lab/lab.cpp
  test/lab_test.cpp)

target_link_libraries(TheLabTest GTest::gtest_main SQLiteCpp sqlite3 pthread dl)

# target_link_libraries(TheLabTest PRIVATE GTest::GTest) if (GMock_FOUND)
# target_link_libraries(TheLabTest INTERFACE GTest::GMock) endif()

include(GoogleTest)
gtest_discover_tests(TheLabTest)
