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

#include "product_iterator.hpp"

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
