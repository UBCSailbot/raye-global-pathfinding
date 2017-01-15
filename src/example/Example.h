// Copyright 2017 UBC Sailbot

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "boost/multi_array.hpp"

class Example {
 public:
  int add(int x, int y);

  boost::multi_array<double, 3, std::allocator<double>> createArray(int x, int y, int z);
};

#endif  // EXAMPLE_H
