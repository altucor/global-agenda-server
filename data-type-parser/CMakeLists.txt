cmake_minimum_required(VERSION 3.15.2)

project(data-type-parser C CXX)
IF(MSVC)
  set(PROJECT_NAME "${PROJECT_NAME}-win")
ELSEIF(APPLE)
  set(PROJECT_NAME "${PROJECT_NAME}-osx")
ELSEIF(UNIX)
  set(PROJECT_NAME "${PROJECT_NAME}-linux")
ENDIF()

set(REQUIRED_BOOST_VERSION 1.75.0)
IF(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MT")
ELSE()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libstdc++")
ENDIF(MSVC)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  message("Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
  message("Release build.")
ELSE()
  message(" ! ! ! Unknown build type.")
ENDIF()

message("CMAKEFLAGS DUMP: ${CMAKE_CXX_FLAGS}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

file(GLOB SOURCES 
    "./main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
IF(MSVC)
    set_property(
        TARGET 
        ${PROJECT_NAME} 
        PROPERTY 
        MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<CONFIG:>:>"
    )
ENDIF(MSVC)
