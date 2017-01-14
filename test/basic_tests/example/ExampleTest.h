#ifndef EXAMPLETEST_H
#define EXAMPLETEST_H

#include <gtest/gtest.h>
#include <example/Example.h>

class ExampleTest : public ::testing::Test  {
 protected:
  ExampleTest();
  Example example;
};

#endif  // EXAMPLETEST_H
