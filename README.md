Product Iterator
=================

C++ iterator that performs the cartesian product of many containers. Requires
C++11 and boost.

All places I could find code to perform the cartesian product had at least one
of the following issues:

1. Assumes that all containers are of a certain type (most usually vector),
   although they could be modified to handle other types easily, but only for
   fixed types;
2. Provides code that collects a vector of tuples with each combination of
   values from the containers, which requires O(N\*M^N) memory, where M is the
   size of each container and N is the number of containers;
3. The user could only apply a function to each combination or would have to
   wrap its code with other code to perform the combinations.

The iterator provided requires only O(N) memory and builds the combinations of
references iteratively, implementing the "forward iterator" concept and allowing
it to be used with other code that requires iterators. The iterator's value is a
tuple, so its values can be accessed as `std::get<N>(*it)`. However, due to some
details to support range-based for, getting the whole tuple is more expensive
and an alternative (and more preferable) method `it.get<N>()` is provided.

The iterator also provides a method `get_end()`, which returns the end iterator
associated with the containers used during construction.

A helper function `make_product_iterator` is provided to create the
iterator.

Example of use:
```
vector<int> c1({1,2});
vector<char> c2({'a','b'});
auto it = make_product_iterator(c1, c2);
auto end = it.get_end();
for (; it != end; ++it) {
  // Provides a std::tuple<int,char> with the values from the constructor
  *it;
  // Access to a single element of the tuple is optimized and the following
  // comparison always holds
  it.get<0>() == std::get<0>(*it);
}
```

Iterator Proxy
---------
The product\_iterator is only able to accept containers to simplify the building
process, as it would be necessary to detect if a given argument is a container
or an iterator. However, since only containers are allowed, iterator-only
sequences, such as those defined by boost::couting\_iterator couldn't be used.
Hence a fake container for these iterators needed to be defined.

The iterator\_proxy is defined only by its begin and end iterators and isn't
able to distinguish between iterator and const\_iterator, so it's up to the user
to know wether the iterator from begin() or end() is actually a const\_iterator.

Example:
```
vector<int> c1({1,2});
auto c2 = make_iterator_proxy(c1.begin(), c2.end());
for (auto v : c2)
  // do stuff
```
