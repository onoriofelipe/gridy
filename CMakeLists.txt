cmake_minimum_required(VERSION 3.0.0)
project(gridey VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost 1.81.0)
# set(CURSES_USE_NCURSES TRUE)
# find_package(Curses)
# if(Boost_FOUND)
#   add_executable(foo foo.cc)
# endif()

include_directories(${Boost_INCLUDE_DIRS})
# include_directories(${CURSES_INCLUDE_DIR})


# none of these work:
# CURSES_INCLUDE_DIR
# C:\msys64\ucrt64\bin
# C:\msys64\ucrt64\include
# C:\msys64\ucrt64\include\ncurses
# set(CURSES_LIBRARY "C:/msys64/ucrt64/bin")
# find_library(CURSES_LIBRARY "C:/msys64/ucrt64/bin/libncurses++w6.dll")
# find_library(CURSES_LIBRARY ncurses "C:/msys64/ucrt64/bin")
# set(CURSES_LIBRARY "C:/msys64/ucrt64/bin/libncurses++w6.dll")
# set(CURSES_INCLUDE_PATH "C:/msys64/ucrt64/include")

include(CTest)
enable_testing()

add_executable(gridy)
target_include_directories(gridy PUBLIC include)
target_sources(gridy PUBLIC   ./main.cpp
                              include/Matrix.h
                              include/AsciiScreen.h
                              include/NormalScreen.h
                              include/Misc.h
                              include/Game.h
                              include/Utils.h
                              include/Enums.h
)
# if(NOT CURSES_FOUND)
   # hardcode because findcurses script is broken for msys
   # set(CURSES_LIBRARIES "C:/msys64/ucrt64/bin/libncursesw6.dll")
# endif()
# target_link_libraries(gridy ${CURSES_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
