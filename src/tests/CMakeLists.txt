set(target tests)

add_executable(${target} 
    tests.cc
    ../model/model.cc
    ../model/credit_model.cc
    ../controller/controller.cc
)

target_link_libraries(${target}
PRIVATE
GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${target})