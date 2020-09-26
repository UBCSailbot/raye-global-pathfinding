project(tests)

# Include Google Test: NOTE(tylerlum): Commented out because it was causing build issues. May only be my system. Need to confirm.
# add_subdirectory(lib/googletest)

# Include test categories
add_subdirectory(basic_tests)
