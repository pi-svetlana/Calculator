cmake_minimum_required(VERSION 3.2)

enable_testing()
project(view VERSION 0.1 LANGUAGES CXX)


add_subdirectory(view)
add_subdirectory(model)
add_subdirectory(controller)
add_subdirectory(external)
add_subdirectory(tests)
