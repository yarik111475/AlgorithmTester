cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME AlgorithmTester)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QWT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rdparty/qwt-6.1.5)
add_subdirectory(${QWT_ROOT})

set(SPDLOG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rdparty/spdlog-1.9.2)
add_subdirectory(${SPDLOG_ROOT})

set(WIN32_LINK_LIBRARIES
    -static-libgcc
    -static-libstdc++
)

set(LINUX_LINK_LIBRARIES
    -static-libgcc
    -static-libstdc++
)

#spdlog
set(SPDLOG_MASTER_PROJECT OFF)
set(SPDLOG_INSTALL OFF)
set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_WCHAR_FILENAMES OFF)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} WIN32
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${QWT_ROOT}/src
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qwt
    spdlog
    ${WIN32_LINK_LIBRARIES}
    ${LINUX_LINK_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
