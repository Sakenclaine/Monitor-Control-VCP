# https://doc.qt.io/qt-6/cmake-get-started.html
# https://stackoverflow.com/questions/25989448/implementing-qt-project-through-cmake

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)


# Set Release Version 
set(RELEASE_SERVICE_VERSION_MAJOR "0")
set(RELEASE_SERVICE_VERSION_MINOR "1")
set(RELEASE_SERVICE_VERSION_MICRO "0")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

# Name Project
project(MonitorControl VERSION ${RELEASE_SERVICE_VERSION})

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set minimal Qt Version and load/find components
set(QT_MIN_VERSION "6.7.0")
find_package (Qt6 REQUIRED COMPONENTS 
	Core 
	Gui
	Widgets
	LinguistTools)

# Standard Qt project setup setting CMake variables
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES de)


# https://stackoverflow.com/questions/68517552/how-to-add-icon-to-a-qt-application-on-windows-using-a-rc-file-on-a-cmake-proje
set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/monitorcontrol.rc")


add_subdirectory( src/app )
#add_subdirectory( src/translation )
add_subdirectory( images )







