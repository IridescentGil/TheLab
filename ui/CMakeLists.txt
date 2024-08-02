set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(TheLab MANUAL_FINALIZATION mainwindow.ui mainwindow.h
                    main.cpp mainwindow.cpp)
  # Define target properties for Android with Qt 6 as: set_property(TARGET
  # TheLab APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  # ${CMAKE_CURRENT_SOURCE_DIR}/android) For more information, see
  # https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(
      TheLab SHARED
      excercise/excercise.cpp
      excercise/excercise.h
      body/body.h
      body/body.cpp
      database/database.h
      database/database.cpp
      history/history.cpp
      history/history.h
      analytics/analytics.h
      analytics/analytics.cpp
      workout/workout.h
      workout/workout.cpp
      lab/lab.h
      lab/lab.cpp)
    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(TheLab mainwindow.ui mainwindow.h main.cpp mainwindow.cpp)
  endif()
endif()

target_include_directories(TheLab PRIVATE ${TheLab_SOURCE_DIR}/src/lab)
target_link_libraries(TheLab PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Lab pthread
                                     dl)
target_compile_features(TheLab PUBLIC cxx_std_23)
if(MSVC)
  target_compile_options(TheLab PRIVATE /W4 /WX)
else()
  target_compile_options(
    TheLab
    PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      "$<$<CONFIG:DEBUG>:-Og;-g3;-fsanitize=address,leak,undefined;-fno-sanitize-recover=all;-fno-optimize-sibling-calls;-fno-omit-frame-pointer;>"
      "$<$<CONFIG:RELEASE>:-O2;>"
      "$<$<AND:$<BOOL:${ENABLE_COVERAGE}>,$<CXX_COMPILER_ID:GNU>>:--coverage;>"
      "$<$<AND:$<BOOL:${ENABLE_COVERAGE}>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate;-fcoverage-mapping;>"
  )
  target_link_options(
    TheLab
    PRIVATE
    "$<$<AND:$<BOOL:${ENABLE_COVERAGE}>,$<CXX_COMPILER_ID:GNU>>:--coverage;>"
    "$<$<AND:$<BOOL:${ENABLE_COVERAGE}>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate;-fcoverage-mapping;>"
    "$<$<CONFIG:DEBUG>:-Og;-g3;-fsanitize=address,leak,undefined;-fno-sanitize-recover=all;-fno-optimize-sibling-calls;-fno-omit-frame-pointer;>"
  )
endif()

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

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(TheLab)
endif()
