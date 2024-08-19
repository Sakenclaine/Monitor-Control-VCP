file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/app/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/app/*.h"
        )

set(PROJECT_SOURCES
    main.cpp
    ${all_SRCS}
    ${app_icon_resource_windows}
)



qt_add_executable(MonitorControl ${PROJECT_SOURCES})


if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_target_properties(MonitorControl
    PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()



target_link_libraries(MonitorControl
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)



