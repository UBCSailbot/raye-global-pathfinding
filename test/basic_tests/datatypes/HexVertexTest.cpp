// Copyright 2017 UBC Sailbot

#include "HexVertexTest.h"

HexVertexTest::HexVertexTest() {}

TEST_F(HexVertexTest, AddTest) {
  HexVertex vertex(Eigen::Vector3f(0, 1, 0));
  for (size_t i = 0; i < HexVertex::kMaxHexVertexNeighbourCount; ++i) {
    EXPECT_EQ(kInvalidHexVertexId, vertex.neighbours[i]);
  }
}