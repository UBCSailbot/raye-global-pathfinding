// Copyright 2017 UBC Sailbot

#include "HexPlanetTest.h"

#include <planet/HexPlanet.h>

HexPlanetTest::HexPlanetTest() {}

TEST_F(HexPlanetTest, HexPlanetCreationTest) {
  HexPlanet hexPlanet = HexPlanet(5);
  EXPECT_EQ(hexPlanet.vertex_count(), 2432);
}
