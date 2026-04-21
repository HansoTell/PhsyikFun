cmake_minimum_required(VERSION 3.20)

project(MyPhysicsLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# GoogleTest 
# =========================
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# =========================
# Subdirectories
# =========================
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)
