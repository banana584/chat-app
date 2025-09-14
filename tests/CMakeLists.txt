enable_testing()

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

add_executable(test_main test_main.c)
# if we need a lib
target_link_libraries(test_main PRIVATE sockets)

if (WIN32)
  target_link_libraries(test_main PRIVATE ws2_32)
endif()

if (MSVC)
  foreach(flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    # Replace any /MT or /MTd with /MD or /MDd
    string(REGEX REPLACE "/MTd" "/MDd" ${flag_var} "${${flag_var}}")
    string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
  endforeach()
endif()

add_test(NAME test_main COMMAND test_main)
