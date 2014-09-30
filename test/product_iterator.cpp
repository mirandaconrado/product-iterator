#include "product_iterator.hpp"
#include "iterator_proxy.hpp"

#include <gtest/gtest.h>

#include <vector>

class ProductIteratorTest: public ::testing::Test {
  protected:
    std::vector<int> v1, v2;
    const int min_i = 1, max_i = 2, min_j = 4, max_j = 6;

    virtual void SetUp() {
      for (int i = min_i; i <= max_i; i++) v1.push_back(i);
      for (int j = min_j; j <= max_j; j++) v2.push_back(j);
    }

    virtual void TearDown() {
      v1.clear();
      v2.clear();
    }
};

TEST_F(ProductIteratorTest, DoubleLoop) {
  auto it = make_product_iterator(v1, v2);
  auto end = it.get_end();

  for (int i = min_i; i <= max_i; i++) {
    for (int j = min_j; j <= max_j; j++) {
      EXPECT_EQ(i, std::get<0>(*it));
      EXPECT_EQ(i, it.get<0>());
      EXPECT_EQ(j, std::get<1>(*it));
      EXPECT_EQ(j, it.get<1>());
      ++it;
    }
  }

  EXPECT_EQ(end, it);
}

TEST_F(ProductIteratorTest, DoubleLoopProxy) {
  auto it = make_product_iterator(make_iterator_proxy(v1.begin(), v1.end()),
        make_iterator_proxy(v2.begin(), v2.end()));
  auto end = it.get_end();

  for (int i = min_i; i <= max_i; i++) {
    for (int j = min_j; j <= max_j; j++) {
      EXPECT_EQ(i, std::get<0>(*it));
      EXPECT_EQ(i, it.get<0>());
      EXPECT_EQ(j, std::get<1>(*it));
      EXPECT_EQ(j, it.get<1>());
      ++it;
    }
  }

  EXPECT_EQ(end, it);
}

TEST_F(ProductIteratorTest, CopyConstructor) {
  auto it = make_product_iterator(v1, v2);
  *it;
  {
    decltype(it) it2(it);
  }
}

TEST_F(ProductIteratorTest, Assignment) {
  auto it = make_product_iterator(v1, v2);
  *it;
  {
    decltype(it) it2;
    it2 = (it);
  }
}
