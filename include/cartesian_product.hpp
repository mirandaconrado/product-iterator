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

// This file defines usefull classes that provides iteration of cartesian
// product of iterable containers, that is, provides a single iterator that
// iterates over all combinations of elements given in the arguments.
//
// The only iterator provided is a constant one, so the user can't change its
// values. This makes sense as changing values would actually change many
// possible iterations, maybe even previous ones.
//
// As writing the class type is hard, a helper function is provided.
//
// Example:
// auto prod = make_cartesian_product(vector<int>({1,2}), vector<int>({3,4}));
// for (auto it = prod.cbegin(); it != prod.cend(); ++it) {
//   // Provides a std::tuple<int,int> with the values from the constructor
//   *it;
//   // Access to a single element of the tuple is optimized and the following
//   // comparison always holds
//   it.get<0>() == std::get<0>(*it);
// }

#ifndef __CARTESIAN_PRODUCT_HPP__
#define __CARTESIAN_PRODUCT_HPP__

#include <tuple>
#include <type_traits>

// Holds a copy of every container provided during construction. For use
// example, see comment at the top of file.
template <class... Types>
class CartesianProduct {
  public:
    typedef std::tuple<
      typename std::remove_reference<Types>::type::value_type const&...
      > value_type;
    typedef std::tuple<
      typename std::remove_reference<Types>::type...
      > container_type;

    // Iterator for the product that provides the basic things one would
    // expect from an iterator. For use example, see comment at the top of
    // file.
    class const_iterator {
      public:
        const_iterator(const_iterator const& other);
        ~const_iterator();
        const_iterator& operator=(const_iterator const& other);
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const_iterator const& other) const;
        bool operator!=(const_iterator const& other) const;

        // Returns a tuple with each value copied from a container iterator.
        value_type const& operator*() const;
        value_type const* operator->() const;

        // Enables access to a single value instead of the whole tuple. This
        // method is preffered to the operator* as it doesn't require tuple
        // construction. Behaves like std::get<I>(*iterator).
        template <size_t I>
          typename std::tuple_element<I, value_type>::type const&
          get() const;

      private:
        // Master class is a friend so it can access the hidden constructor and
        // tuple of iterators.
        friend CartesianProduct;

        // Constructor that enables creation of new (non-copy) iterators. Only
        // Process should access it as the base_container is the container
        // inside of it.
        const_iterator(container_type const* base_container);

        // Holds the last tuple created by operator*, so that it can be returned
        // in multiple calls if the iterator is the same. This variable if freed
        // in operator++ and the destructor.
        // We must use this so that we can return a const& at operator* to
        // comply with the range-based for
        // for (auto& it : product)
        // If it could be "auto it", then we wouldn't need to keep reallocating
        // this thing. We also can't keep a single copy because each element of
        // the tuple is a const&, so it can't be constructed by itself.
        mutable value_type* current_tuple_;

        std::tuple<
          typename std::remove_reference<Types>::type::const_iterator...
          > it_tuple_;

        container_type const* containers_;
    };

    // The arguments must be containers that provide value_type and
    // const_iterator.
    CartesianProduct(Types&&... containers);

    const_iterator const& cbegin() const;
    const_iterator const& cend() const;

  private:
    // Methods to copy containers arguments into internal tuples.
    template <size_t I, class T>
      void CopyContainers(T const& container);
    template <size_t I, class T>
      void CopyContainers(T&& container);
    template <size_t I, class T, class... Types_>
      void CopyContainers(T const& container, Types_&&... containers);
    template <size_t I, class T, class... Types_>
      void CopyContainers(T&& container, Types_&&... containers);

    container_type containers_;
    const_iterator begin_, end_;
};

template <class... Types>
CartesianProduct<Types...> make_cartesian_product(Types&&... containers) {
  return CartesianProduct<Types...>(std::forward<Types>(containers)...);
}

#include "cartesian_product_impl.hpp"

#endif
