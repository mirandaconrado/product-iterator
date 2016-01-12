include_directories(${GTEST_INCLUDE_DIRS})

add_executable(product_iterator_test EXCLUDE_FROM_ALL
  product_iterator.cpp
)

target_link_libraries(product_iterator_test ${GTEST_BOTH_LIBRARIES})

GTEST_ADD_TESTS(product_iterator_test "" product_iterator.cpp)
