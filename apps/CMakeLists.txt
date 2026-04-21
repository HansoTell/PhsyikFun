file(GLOB APP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

foreach(app_source ${APP_SOURCES})
    # Dateiname ohne Pfad + ohne .cpp
    get_filename_component(app_name ${app_source} NAME_WE)

    add_executable(${app_name} ${app_source})

    target_link_libraries(${app_name}
        PRIVATE MyPhysicsLib
    )
endforeach()
