cartesian-product
=================

C++ container that performs the cartesian product of many containers, and
requires C++11. GCC 4.8.3 fails to compile the test case for some reason.

All places I could find code to perform the cartesian product had at least one
of the following issues:

1. Assumes that all containers are of a certain type (most usually vector),
although they could be modified to handle other types easily;
2. Provides code that collects a vector of tuples with each combination of
values from the containers, which requires O(M^N) memory, where M is the size of
each container and N is the number of containers;
3. The user could only apply a function to each combination or would have to
wrap its code with other code to perform the combinations.

This container is designed to be used like any other, making it very easy for
the user. Only a constant iterator is provided, as a modifiable one doesn't make
much sense to me. The iterator's value is a tuple, so its values can be accessed
as `std::get<N>(*it)`. However, due to some details to support range-based for,
getting the whole tuple is more expensive and an alternative (and more
preferable) accessor `it.get<N>()` is provided.

If any of the containers provided are MoveAssignable, then it's moved.
Otherwise, it has to be copied to avoid invalid references.

A helper function `make_cartesian_product` is provided to create the
CartesianProduct object.

Example of use:
```
auto prod = make_cartesian_product(vector<int>({1,2}), vector<int>({3,4}));
for (auto it = prod.begin(); it != prod.end(); ++it) {
  // Provides a std::tuple<int,int> with the values from the constructor
  *it;
  // Access to a single element of the tuple is optimized and the following
  // comparison always holds
  it.get<0>() == std::get<0>(*it);
}
```
