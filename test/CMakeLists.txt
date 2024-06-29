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
include_directories(
  utils
  ${TheLab_SOURCE_DIR}/src/excercise
  ${TheLab_SOURCE_DIR}/src/body
  ${TheLab_SOURCE_DIR}/src/database
  ${TheLab_SOURCE_DIR}/src/analytics
  ${TheLab_SOURCE_DIR}/src/history
  ${TheLab_SOURCE_DIR}/src/workout
  ${TheLab_SOURCE_DIR}/src/lab)

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

target_link_libraries(TheLabUnitTest PRIVATE GTest::gtest_main Lab SQLiteCpp
                                             sqlite3 pthread dl)
gtest_discover_tests(TheLabUnitTest)
