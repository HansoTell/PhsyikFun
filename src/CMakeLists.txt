# alle cpp Dateien automatisch sammeln
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(MyPhysicsLib ${LIB_SOURCES})

target_include_directories(MyPhysicsLib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
