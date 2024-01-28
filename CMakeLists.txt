cmake_minimum_required(VERSION 3.10)
project(GPP)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(DOWNLOAD "https://raw.githubusercontent.com/mylogin/htmlparser/main/html.hpp" ${CMAKE_SOURCE_DIR}/src/html.hpp)
file(DOWNLOAD "https://raw.githubusercontent.com/mylogin/htmlparser/main/html.cpp" ${CMAKE_SOURCE_DIR}/src/html.cpp)

# Укажите исходные файлы вашей библиотеки
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/html.cpp ${CMAKE_SOURCE_DIR}/src/html.hpp
    ${CMAKE_SOURCE_DIR}/src/search.h ${CMAKE_SOURCE_DIR}/src/search.cpp
    ${CMAKE_SOURCE_DIR}/src/parcel_storage.h ${CMAKE_SOURCE_DIR}/src/parcel_storage.cpp
    ${CMAKE_SOURCE_DIR}/src/parcel.h
    ${CMAKE_SOURCE_DIR}/src/gpp.h
    ${CMAKE_SOURCE_DIR}/src/info_storage.h
)

if(GPP_LIBRARY_BUILD_STATIC)
    add_library(GPP STATIC ${SOURCES})
else()
    add_library(GPP SHARED ${SOURCES})
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

target_link_libraries(GPP PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)
