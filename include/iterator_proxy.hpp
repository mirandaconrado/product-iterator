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
