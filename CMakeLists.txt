cmake_minimum_required(VERSION 3.0.0)
project(gridey VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost 1.81.0)
# if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
#   add_executable(foo foo.cc)
# endif()

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
                              )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
