set(target model)

add_library(${target}
    STATIC
    model.cc
    model.h
    credit_model.h
    credit_model.cc)

include_directories(.)
