set(target controller)

add_library(${target}
    STATIC
    controller.cc
    controller.h)

include_directories(.)