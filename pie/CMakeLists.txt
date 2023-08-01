cmake_minimum_required(VERSION 3.5)

project(pie LANGUAGES C)
list(APPEND CMAKE_MODULE_PATH "/usr/include")
find_package(gd)
add_executable(pie main.c)
target_link_libraries(pie gd)
install(TARGETS pie
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
