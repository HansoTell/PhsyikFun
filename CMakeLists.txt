cmake_minimum_required(VERSION 3.20)
project(MyPhysicsLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library + Subdirs
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)
