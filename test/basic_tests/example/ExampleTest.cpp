// Copyright 2017 UBC Sailbot

#include "ExampleTest.h"
ExampleTest::ExampleTest() {

}

TEST_F(ExampleTest, AddTest) {
  example.add(2, 2);
  EXPECT_EQ(4, example.add(2, 2));
}
TEST_F(ExampleTest, ArrayTest) {
  auto array = example.createArray(5, 6, 7);

  int verify = 0;
  for (int i = 0; i != 5; ++i)
    for (int j = 0; j != 6; ++j)
      for (int k = 0; k != 7; ++k)
        EXPECT_EQ(array[i][j][k], verify++);
}