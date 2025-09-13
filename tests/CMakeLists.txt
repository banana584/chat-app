enable_testing()

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

set(CMAKE_C_FLAGS_DEBUG "-g -fsanitize=address")

add_executable(test_main test_main.c)
# if we need a lib
target_link_libraries(test_main PRIVATE sockets)

add_test(NAME test_main COMMAND test_main)
