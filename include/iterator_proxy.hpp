// This file defines a fake container that is only defined by a pair of
// iterators.
//
// The product_iterator is only able to accept containers to simplify the
// building process, as it would be necessary to detect if a given argument is a
// container or an iterator. However, since only containers are allowed,
// iterator-only sequences, such as those defined by boost::couting_iterator
// couldn't be used. Hence a fake container for these iterators needed to be
// defined.
//
// The iterator_proxy is defined only by its begin and end iterators and isn't
// able to distinguish between iterator and const_iterator, so it's up to the
// user to know wether the iterator from begin() or end() is actually a
// const_iterator.
//
// Example:
// vector<int> c1({1,2});
// auto c2 = make_iterator_proxy(c1.begin(), c2.end());
// for (auto v : c2)
//   // do stuff

#ifndef __ITERATOR_PROXY_HPP__
#define __ITERATOR_PROXY_HPP__

#include <cstddef>

template <class It>
class iterator_proxy {
  public:
    typedef typename It::value_type value_type;
    typedef typename It::value_type& reference;
    typedef typename It::value_type const& const_reference;
    typedef typename It::value_type* pointer;
    typedef typename It::value_type const* const_pointer;
    typedef It iterator;
    typedef It const_iterator;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;

    iterator_proxy(It const& b, It const& e):
      begin_(b),
      end_(e) { }

    It begin() const { return begin_; }
    It end() const { return end_; }

    It cbegin() const { return begin_; }
    It cend() const { return end_; }

  private:
    It begin_, end_;
};

template <class It>
iterator_proxy<It> make_iterator_proxy(It const& begin, It const& end) {
  return iterator_proxy<It>(begin, end);
}

#endif
