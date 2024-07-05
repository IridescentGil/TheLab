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

include(GoogleTest)

add_executable(
  TheLabUnitTest
  utils/testHelper.cpp
  unit_tests/excercise_test.cpp
  unit_tests/database_test.cpp
  unit_tests/body_test.cpp
  unit_tests/history_test.cpp
  unit_tests/analytics_test.cpp
  unit_tests/workout_test.cpp
  unit_tests/lab_test.cpp)

target_include_directories(TheLabUnitTest PRIVATE utils)

target_link_libraries(TheLabUnitTest PRIVATE GTest::gtest_main Lab SQLiteCpp
                                             sqlite3 pthread dl)

target_compile_features(TheLabUnitTest PUBLIC cxx_std_23)
if(MSVC)
  target_compile_options(TheLabUnitTest PRIVATE /W4 /WX)
else()
  target_compile_options(
    TheLabUnitTest
    PRIVATE -Wall -Wextra -Wpedantic "$<$<CONFIG:DEBUG>:-O0;-g3;>"
            "$<$<CONFIG:RELEASE>:-O2;>"
            "$<$<BOOL:${ENABLE_COVERAGE}>:--coverage;>")
  target_link_options(TheLabUnitTest PRIVATE
                      "$<$<BOOL:${ENABLE_COVERAGE}>:--coverage;>")
endif()
gtest_discover_tests(TheLabUnitTest)
