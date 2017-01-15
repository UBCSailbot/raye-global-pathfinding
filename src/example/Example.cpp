// Copyright 2017 UBC Sailbot

#include <iostream>
#include "Example.h"

int Example::add(int x, int y) {
  return x + y;
}

boost::multi_array<double, 3, std::allocator<double>> Example::createArray(int x, int y, int z) {
  boost::multi_array<double, 3> array(boost::extents[x][y][z]);

  // Assign values to the elements
  int values = 0;
  for (int i = 0; i != x; ++i)
    for (int j = 0; j != y; ++j)
      for (int k = 0; k != z; ++k)
      array[i][j][k] = values++;
  return array;
}
