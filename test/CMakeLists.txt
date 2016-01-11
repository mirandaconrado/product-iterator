include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_tests.bin EXCLUDE_FROM_ALL
  product_iterator.cpp
)

target_link_libraries(run_tests.bin ${GTEST_BOTH_LIBRARIES})

add_custom_target(test COMMAND run_tests.bin
                       DEPENDS run_tests.bin)
