cmake_minimum_required(VERSION 3.16)
project(ChatApp VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

if (MSCV)
  foreach(flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    # Replace any /MT or /MTd with /MD or /MDd
    string(REGEX REPLACE "/MTd" "/MDd" ${flag_var} "${${flag_var}}")
    string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
  endforeach()
endif()

add_executable(${CMAKE_PROJECT_NAME} 
    src/main.c
)

add_library(sockets
    src/sockets/sockets.c
)

target_include_directories(sockets PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_link_libraries(ChatApp PRIVATE ws2_32 ucrt)
    target_link_libraries(sockets PRIVATE ws2_32 ucrt)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sockets)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
