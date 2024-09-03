get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")



message("\n\nPathes:")
message("${WINDEPLOYQT_EXECUTABLE}")
message("${PROJECT_SOURCE_DIR}")
message("${CMAKE_CURRENT_BINARY_DIR}")
message("${CMAKE_INSTALL_BINDIR}\n\n")


qt_add_translations(MonitorControl 
    TS_FILE_DIR ${PROJECT_SOURCE_DIR}/src/translation)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/app/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/app/*.h"
        )


set(PROJECT_SOURCES
    main.cpp
    ${all_SRCS}
    ${app_icon_resource_windows}
)



qt_add_executable(MonitorControl ${PROJECT_SOURCES} ${qm_files})


target_link_libraries(MonitorControl
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)





# https://blog.nathanosman.com/2017/11/24/using-windeployqt-with-cpack.html
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#add_custom_command(TARGET MonitorControl POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E
#        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
#           \"$<TARGET_FILE:MonitorControl>\"
#    COMMENT "Deploying Qt..."
#)
#endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_target_properties(MonitorControl
    PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

    install(TARGETS MonitorControl
        BUNDLE  DESTINATION "P:\\MonitorControl"
        RUNTIME DESTINATION "P:\\MonitorControl"
    )

    qt_generate_deploy_app_script(
        TARGET MonitorControl
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
endif()




# For some reason at one point something got written to 
# ....\6.7.1\msvc2019_64\lib\cmake\Qt6Core\Qt6CoreMacros.cmake



