cmake_minimum_required(VERSION 3.15.2)

project(global-agenda-server C CXX)
IF(MSVC)
  set(PROJECT_NAME "${PROJECT_NAME}-win")
ELSEIF(APPLE)
  set(PROJECT_NAME "${PROJECT_NAME}-osx")
ELSEIF(UNIX)
  set(PROJECT_NAME "${PROJECT_NAME}-linux")
ENDIF()

set(REQUIRED_BOOST_VERSION 1.75.0)
set(REQUIRED_MBEDTLS_VERSION 2.25.0)
set(REQUIRED_OPENSSL_VERSION 1.1.1g)
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

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES 
                      boost/${REQUIRED_BOOST_VERSION} 
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED)

include_directories("./include")

file(GLOB SOURCES 
    "./include/Utils.hpp"
    "./include/EnumDataTypes.hpp"
    "./include/ValueConverter.hpp"
    "./include/DataEntry.hpp"
    "./include/DataType.hpp"
    "./include/DataTypesHolder.hpp"
    "./include/Logger.hpp"
    "./include/AnonymousArray.hpp"
    "./include/EntryHeader.hpp"
    "./include/DataSet.hpp"
    "./include/Packet.hpp"
    "./include/TcpPacket.hpp"
    "./include/Server.hpp"
    "./src/Utils.cpp"
    "./src/DataType.cpp"
    "./src/DataTypesHolder.cpp"
    "./src/Logger.cpp"
    "./src/ValueConverter.cpp"
    "./src/EntryHeader.cpp"
    "./src/DataEntry.cpp"
    "./src/AnonymousArray.cpp"
    "./src/DataSet.cpp"
    "./src/Packet.cpp"
    "./src/TcpPacket.cpp"
    "./src/Client.cpp"
    "./src/Server.cpp"
    "./src/main.cpp"
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

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Boost::Boost)
