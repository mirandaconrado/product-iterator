/*
The MIT License (MIT)

Copyright (c) 2014 Conrado Miranda

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "cartesian_product.hpp"

#include <gtest/gtest.h>

#include <vector>

class CartesianProductTest: public ::testing::Test {
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

TEST_F(CartesianProductTest, PermanentObjectsUsedInConstruction) {
  auto prod = make_cartesian_product(v1, v2);

  // Changes to make sure the product copied them
  for (auto& it : v1) it *= 2;
  for (auto& it : v2) it *= 2;

  auto it = prod.cbegin();

  for (int j = min_j; j <= max_j; j++) {
    for (int i = min_i; i <= max_i; i++) {
      ASSERT_EQ(i, std::get<0>(*it));
      ASSERT_EQ(i, it.get<0>());
      ASSERT_EQ(j, std::get<1>(*it));
      ASSERT_EQ(j, it.get<1>());
      ++it;
    }
  }

  ASSERT_EQ(prod.cend(), it);
}

TEST_F(CartesianProductTest, TemporaryObjectsUsedInConstruction) {
  auto prod = make_cartesian_product(std::move(v1), std::move(v2));

  auto it = prod.cbegin();

  for (int j = min_j; j <= max_j; j++) {
    for (int i = min_i; i <= max_i; i++) {
      ASSERT_EQ(i, std::get<0>(*it));
      ASSERT_EQ(i, it.get<0>());
      ASSERT_EQ(j, std::get<1>(*it));
      ASSERT_EQ(j, it.get<1>());
      ++it;
    }
  }

  ASSERT_EQ(prod.cend(), it);
}

TEST_F(CartesianProductTest, IteratorDefaultConstructible) {
  auto prod = make_cartesian_product(std::move(v1), std::move(v2));

  decltype(prod)::const_iterator it;
}

TEST_F(CartesianProductTest, IteratorComparison) {
  auto prod = make_cartesian_product(std::move(v1), std::move(v2));

  for (auto it1 = prod.cbegin(), it2 = prod.cbegin();
       it1 != prod.cend();
       ++it1) {
    ASSERT_EQ(it1, it2++);
    ASSERT_NE(it1, it2);
  }
}
