cmake_minimum_required(VERSION 3.11)
project(GPP)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(DOWNLOAD "https://raw.githubusercontent.com/mylogin/htmlparser/main/html.hpp" ${GPP_SOURCE_DIR}/src/gpp/html.hpp)
file(DOWNLOAD "https://raw.githubusercontent.com/mylogin/htmlparser/main/html.cpp" ${GPP_SOURCE_DIR}/src/gpp/html.cpp)

# Укажите исходные файлы вашей библиотеки
set(SOURCES
    ${GPP_SOURCE_DIR}/src/gpp/html.cpp ${GPP_SOURCE_DIR}/src/gpp/html.hpp
    ${GPP_SOURCE_DIR}/src/gpp/search.h ${GPP_SOURCE_DIR}/src/gpp/search.cpp
    ${GPP_SOURCE_DIR}/src/gpp/parcel_storage.h ${GPP_SOURCE_DIR}/src/gpp/parcel_storage.cpp
    ${GPP_SOURCE_DIR}/src/gpp/parcel.h
    ${GPP_SOURCE_DIR}/src/gpp/gpp.h
    ${GPP_SOURCE_DIR}/src/gpp/info_storage.h
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
