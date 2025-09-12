enable_testing()

add_executable(test_main test_main.c)
# if we need a lib
# target_link_libraries(test_example PRIVATE mylib)

add_test(NAME test_main COMMAND test_main)
