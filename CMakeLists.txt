cmake_minimum_required(VERSION 3.16)
project(ChatApp VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

add_executable(${CMAKE_PROJECT_NAME} 
    src/main.c
)

add_library(sockets
    src/sockets/sockets.c
)

add_library(server
    src/server/server.c
)

target_include_directories(sockets PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    target_link_libraries(ChatApp PRIVATE ws2_32 ucrt)
    target_link_libraries(sockets PRIVATE ws2_32 ucrt)
endif()

target_link_libraries(server PRIVATE sockets)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sockets)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
