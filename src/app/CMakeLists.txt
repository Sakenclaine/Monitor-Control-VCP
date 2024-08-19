file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/app/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/app/*.h"
        )

set(PROJECT_SOURCES
    main.cpp
    ${all_SRCS}
)



qt_add_executable(Barebone ${PROJECT_SOURCES})


if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_target_properties(Barebone
    PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()



target_link_libraries(Barebone
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)



