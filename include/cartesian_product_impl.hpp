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

// Implementation details for the CartesianProduct class.

#ifndef __CARTESIAN_PRODUCT_IMPL_HPP__
#define __CARTESIAN_PRODUCT_IMPL_HPP__

#include "cartesian_product.hpp"

template <class... Types>
CartesianProduct<Types...>::const_iterator::const_iterator():
  current_tuple_(nullptr),
  containers_(nullptr) { }

template <class... Types>
CartesianProduct<Types...>::const_iterator::const_iterator(
    const_iterator const& other):
  current_tuple_(nullptr),
  it_tuple_(other.it_tuple_),
  containers_(other.containers_) { }

template <class... Types>
CartesianProduct<Types...>::const_iterator::const_iterator(
    container_type const* base_container):
  current_tuple_(nullptr),
  containers_(base_container) { }

template <class... Types>
CartesianProduct<Types...>::const_iterator::~const_iterator() {
  if (current_tuple_ != nullptr)
    delete current_tuple_;
}

template <class... Types>
typename CartesianProduct<Types...>::const_iterator&
CartesianProduct<Types...>::const_iterator::operator=(
    const_iterator const& other) {
  it_tuple_ = other.it_tuple_;
  containers_ = other.containers_;
  return *this;
}

// Helper methods to operator++.
template <size_t I, class T1, class T2>
typename std::enable_if<(I == std::tuple_size<T1>::value), void>::type
advance(T1& it_tuple, T2& containers) { }

// Advances iterator at position I. If it gets to the end of the containers, 2
// things can happen:
// 1) this is the last iterator, which means we have gone over all possible
// combinations. Then just return and leaver the iterator at the end, as this is
// how it will equal the final iterator.
// 2) this isn't the last iterator, so it resets it to begin and advances the
// next iterator.
template <size_t I, class T1, class T2>
typename std::enable_if<(I < std::tuple_size<T1>::value), void>::type
advance(T1& it_tuple, T2& containers) {
  ++std::get<I>(it_tuple);

  if (std::get<I>(it_tuple) == std::get<I>(containers).cend()) {
    if (I == std::tuple_size<T1>::value-1)
      return;

    std::get<I>(it_tuple) = std::get<I>(containers).cbegin();
    advance<I + 1>(it_tuple, containers);
  }
}

template <class... Types>
typename CartesianProduct<Types...>::const_iterator&
CartesianProduct<Types...>::const_iterator::operator++() {
  if (containers_ == nullptr)
    return *this;

  // Avoids incrementing if we have already reached the end.
  if (std::get<sizeof...(Types)-1>(it_tuple_) ==
      std::get<sizeof...(Types)-1>(*containers_).cend())
    return *this;

  advance<0>(it_tuple_, *containers_);
  if (current_tuple_ != nullptr) {
    delete current_tuple_;
    current_tuple_ = nullptr;
  }
  return *this;
}

template <class... Types>
typename CartesianProduct<Types...>::const_iterator
CartesianProduct<Types...>::const_iterator::operator++(int) {
  const_iterator it(*this);
  ++(*this);
  return it;
}

template <class... Types>
bool CartesianProduct<Types...>::const_iterator::operator==(
    const_iterator const& other) const {
  return other.it_tuple_ == it_tuple_;
}

template <class... Types>
bool CartesianProduct<Types...>::const_iterator::operator!=(
    const_iterator const& other) const {
  return !(*this == other);
}

// Helper methods to operator* and operator->. Just collect each tuple position
// of value_type as a reference to the current position.
template <size_t I, class Ret, class T,
          typename std::enable_if<(I==std::tuple_size<T>::value-1),int>::type=0>
auto make_value_type(T const& tuple) ->
decltype(
    std::tuple<
    typename std::tuple_element<I,Ret>::type const&
    >(*std::get<I>(tuple)))
{
  return std::tuple<
    typename std::tuple_element<I,Ret>::type const&
    >(*std::get<I>(tuple));
}

template <size_t I, class Ret, class T,
          typename std::enable_if<(I<std::tuple_size<T>::value-1),int>::type=0>
auto make_value_type(T const& tuple) ->
decltype(std::tuple_cat(
      std::tuple<
      typename std::tuple_element<I,Ret>::type const&
      >(*std::get<I>(tuple)),
      make_value_type<I+1,Ret>(tuple)))
{
  return std::tuple_cat(
      std::tuple<
      typename std::tuple_element<I,Ret>::type const&
      >(*std::get<I>(tuple)),
      make_value_type<I+1,Ret>(tuple));
}

template <class... Types>
typename CartesianProduct<Types...>::value_type const&
CartesianProduct<Types...>::const_iterator::operator*() const {
  return *operator->();
}

template <class... Types>
typename CartesianProduct<Types...>::value_type const*
CartesianProduct<Types...>::const_iterator::operator->() const {
  if (current_tuple_ == nullptr)
    current_tuple_ = new value_type(make_value_type<0,value_type>(it_tuple_));
  return current_tuple_;
}

template <class... Types>
template <size_t I>
typename
std::tuple_element<I, typename CartesianProduct<Types...>::value_type>::type
const &
CartesianProduct<Types...>::const_iterator::get() const {
  // As only a single value is needed, gathers it from its iterator.
  return *std::get<I>(it_tuple_);
}

// Copies the containers and the begin of each one of them. Also creates the end
// iterator, which is a copy of the begin, except that the last iterator is at
// the end of its container.
template <class... Types>
CartesianProduct<Types...>::CartesianProduct(Types&&... containers):
  begin_(&containers_),
  end_(&containers_) {
    CopyContainers<0>(std::forward<Types>(containers)...);
    end_ = begin_;
    std::get<sizeof...(Types)-1>(end_.it_tuple_) =
      std::get<sizeof...(Types)-1>(containers_).cend();
  }

template <class... Types>
template <size_t I, class T>
void CartesianProduct<Types...>::CopyContainers(T const& container) {
  std::get<I>(containers_) = container;
  // Gets the begin from the local copy. Don't use container.begin() as it may
  // become invalid!
  std::get<I>(begin_.it_tuple_) = std::get<I>(containers_).cbegin();
}

template <class... Types>
template <size_t I, class T>
void CartesianProduct<Types...>::CopyContainers(T&& container) {
  std::get<I>(containers_) = std::move(container);
  // Gets the begin from the local copy. Don't use container.begin() as it may
  // become invalid!
  std::get<I>(begin_.it_tuple_) = std::get<I>(containers_).cbegin();
}

template <class... Types>
template <size_t I, class T, class... Types_>
void CartesianProduct<Types...>::CopyContainers(T const& container,
    Types_&&... containers) {
  std::get<I>(containers_) = container;
  // Gets the begin from the local copy. Don't use container.begin() as it may
  // become invalid!
  std::get<I>(begin_.it_tuple_) = std::get<I>(containers_).cbegin();
  CopyContainers<I + 1>(std::forward<Types_>(containers)...);
}

template <class... Types>
template <size_t I, class T, class... Types_>
void CartesianProduct<Types...>::CopyContainers(T&& container,
    Types_&&... containers) {
  std::get<I>(containers_) = std::move(container);
  // Gets the begin from the local copy. Don't use container.begin() as it may
  // become invalid!
  std::get<I>(begin_.it_tuple_) = std::get<I>(containers_).cbegin();
  CopyContainers<I + 1>(std::forward<Types_>(containers)...);
}

template <class... Types>
typename CartesianProduct<Types...>::const_iterator const&
CartesianProduct<Types...>::cbegin() const {
  return begin_;
}

template <class... Types>
typename CartesianProduct<Types...>::const_iterator const&
CartesianProduct<Types...>::cend() const {
  return end_;
}

#endif
