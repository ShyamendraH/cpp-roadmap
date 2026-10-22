add_executable(srcscope
    src/main.cpp)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
