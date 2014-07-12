#include "cartesian_product.hpp"

#include <gtest/gtest.h>

#include <vector>

TEST(CartesianProduct, PermanentObjectsUsedInConstruction) {
  std::vector<int> v1({1,2});
  std::vector<int> v2({4,5,6});
  auto prod = make_cartesian_product(v1, v2);

  auto it = prod.begin();

  for (int j = 4; j <= 6; j++) {
    for (int i = 1; i <= 2; i++) {
      ASSERT_EQ(i, std::get<0>(*it));
      ASSERT_EQ(i, it.get<0>());
      ASSERT_EQ(j, std::get<1>(*it));
      ASSERT_EQ(j, it.get<1>());
      ++it;
    }
  }

  ASSERT_EQ(prod.end(), it);
}

TEST(CartesianProduct, TemporaryObjectsUsedInConstruction) {
  auto prod = make_cartesian_product(std::vector<int>({1,2}),
                                     std::vector<int>({4,5,6}));

  auto it = prod.begin();

  for (int j = 4; j <= 6; j++) {
    for (int i = 1; i <= 2; i++) {
      ASSERT_EQ(i, std::get<0>(*it));
      ASSERT_EQ(i, it.get<0>());
      ASSERT_EQ(j, std::get<1>(*it));
      ASSERT_EQ(j, it.get<1>());
      ++it;
    }
  }

  ASSERT_EQ(prod.end(), it);
}
