find_package(SQLiteCpp REQUIRED)

add_library(
  Lab SHARED
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

target_include_directories(
  Lab
  PUBLIC excercise
         body
         database
         analytics
         history
         workout
         lab)

target_link_libraries(Lab PRIVATE SQLiteCpp sqlite3)

target_compile_features(Lab PUBLIC cxx_std_23)
if(MSVC)
  target_compile_options(Lab PRIVATE /W4 /WX)
else()
  target_compile_options(
    Lab
    PRIVATE -Wall -Wextra -Wpedantic "$<$<CONFIG:DEBUG>:-O0;-g3;>"
            "$<$<CONFIG:RELEASE>:-O2;>"
            "$<$<BOOL:${ENABLE_COVERAGE}>:--coverage;>")
  target_link_options(Lab PRIVATE "$<$<BOOL:${ENABLE_COVERAGE}>:--coverage;>")
endif()

option(DOCUMENTATION "Create Doxygen documentation" ON)
if(DOCUMENTATION)
  find_package(Doxygen REQUIRED dot)
  if(DOXYGEN_FOUND)
    doxygen_add_docs(DOCS ${LIBRARY_SOURCES} ALL)
  endif()
endif()
