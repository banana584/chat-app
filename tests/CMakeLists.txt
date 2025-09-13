enable_testing()

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

if (WIN32)
  target_link_libraries(test_main PRIVATE ws2_32)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g -fsanitize=address")

add_executable(test_main test_main.c)
# if we need a lib
target_link_libraries(test_main PRIVATE sockets)

if (MSVC)
    target_compile_options(test_main PRIVATE /fsanitize=address /Zi /Od)
    target_link_options(test_main PRIVATE /fsanitize=address /DEBUG)
endif()

add_test(NAME test_main COMMAND test_main)
