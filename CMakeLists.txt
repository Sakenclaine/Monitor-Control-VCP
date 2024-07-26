# https://doc.qt.io/qt-6/cmake-get-started.html
# https://stackoverflow.com/questions/25989448/implementing-qt-project-through-cmake

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)


# Set Release Version 
set(RELEASE_SERVICE_VERSION_MAJOR "0")
set(RELEASE_SERVICE_VERSION_MINOR "1")
set(RELEASE_SERVICE_VERSION_MICRO "0")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

# Name Project
project(MonitorControl_2 VERSION ${RELEASE_SERVICE_VERSION})

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set minimal Qt Version and load/find components
set(QT_MIN_VERSION "6.7.0")
find_package (Qt6 REQUIRED COMPONENTS 
	Core 
	Gui
	Widgets)

# Standard Qt project setup setting CMake variables
qt_standard_project_setup()

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/monitorcontrol.rc")

qt_add_resources(helloworld imageresources
    PREFIX "/icons"
    FILES monitor.ico
)

add_subdirectory( src/app ) 
add_subdirectory( icons )





