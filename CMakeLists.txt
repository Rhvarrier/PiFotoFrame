cmake_minimum_required(VERSION 3.14)

project(PiFotoFrame LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt5_DIR $ENV{QT_CMAKE_DIR}/Qt5)
find_package(QT NAMES Qt5)
include_directories(${QtQml_INCLUDE_DIRS})
find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)
add_definitions(${QtQml_DEFINITIONS} ${${Qt${QT_VERSION_MAJOR}Quick_DEFINITIONS}})

file (GLOB SRC_FILES 
	src/*.cpp
	src/*.hpp
	res/qml.qrc
	)
add_executable(${CMAKE_PROJECT_NAME}
  ${SRC_FILES}
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC src/)
target_link_libraries(${CMAKE_PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
