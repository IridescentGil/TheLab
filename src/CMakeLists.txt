find_package(SQLiteCpp REQUIRED)

set(LIBRARY_SOURCES
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

include_directories(
  excercise
  body
  database
  analytics
  history
  workout
  lab)

add_library(Lab SHARED ${LIBRARY_SOURCES})
target_link_libraries(Lab PRIVATE SQLiteCpp sqlite3)

option(DOCUMENTATION "Create Doxygen documentation" ON)
if(DOCUMENTATION)
  find_package(Doxygen REQUIRED dot)
  if(DOXYGEN_FOUND)
    doxygen_add_docs(DOCS ${LIBRARY_SOURCES} ALL)
  endif()
endif()
