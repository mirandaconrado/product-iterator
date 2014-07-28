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

TEST(CartesianProductTest, PermanentObjectsUsedInConstruction) {
  std::vector<int> v1({1,2});
  std::vector<int> v2({4,5,6});
  auto prod = make_cartesian_product(v1, v2);

  auto it = prod.cbegin();

  for (int j = 4; j <= 6; j++) {
    for (int i = 1; i <= 2; i++) {
      ASSERT_EQ(i, std::get<0>(*it));
      ASSERT_EQ(i, it.get<0>());
      ASSERT_EQ(j, std::get<1>(*it));
      ASSERT_EQ(j, it.get<1>());
      ++it;
    }
  }

  ASSERT_EQ(prod.cend(), it);
}

TEST(CartesianProductTest, TemporaryObjectsUsedInConstruction) {
  auto prod = make_cartesian_product(std::vector<int>({1,2}),
                                     std::vector<int>({4,5,6}));

  auto it = prod.cbegin();

  for (int j = 4; j <= 6; j++) {
    for (int i = 1; i <= 2; i++) {
      ASSERT_EQ(i, std::get<0>(*it));
      ASSERT_EQ(i, it.get<0>());
      ASSERT_EQ(j, std::get<1>(*it));
      ASSERT_EQ(j, it.get<1>());
      ++it;
    }
  }

  ASSERT_EQ(prod.cend(), it);
}

TEST(CartesianProductTest, IteratorDefaultConstructible) {
  auto prod = make_cartesian_product(std::vector<int>({1,2}),
                                     std::vector<int>({4,5,6}));

  decltype(prod)::const_iterator it;
}

TEST(CartesianProductTest, IteratorComparison) {
  auto prod = make_cartesian_product(std::vector<int>({1,2}),
                                     std::vector<int>({4,5,6}));

  for (auto it1 = prod.cbegin(), it2 = prod.cbegin();
       it1 != prod.cend();
       ++it1) {
    ASSERT_EQ(it1, it2++);
    ASSERT_NE(it1, it2);
  }
}
