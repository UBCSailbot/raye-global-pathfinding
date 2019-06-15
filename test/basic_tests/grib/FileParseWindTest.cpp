// Copyright 2017 UBC Sailbot

#include "FileParseWindTest.h"
#include "grib/gribParse.h"
#include <eccodes.h>
#include <math.h>
#include "grib/windFileParse.h"

WindGribParseTest::WindGribParseTest() {}

TEST_F(WindGribParseTest, TestMagnitude) {
    struct Test {
        double knot_output, u_comp, v_comp;
    };

    const std::vector<Test> test_data = {
            {0, 0, 0},
            {27.549, 10, 10},
            {1953.54, 1000, 100},
            {1182.39, -100, -600},
            {1, 0.514444, 0}
    };

    for (const auto &test : test_data) {
        double expected = test.knot_output;
        double output = gribParse::calcMagnitude(test.u_comp, test.v_comp);
        EXPECT_NEAR(expected, output, 0.1)
                            << "Test calcMagnitude(" << test.u_comp << ", " << test.v_comp << ") failed." << std::endl;
    }
}

TEST_F(WindGribParseTest, TestAngle) {
    struct Test {
        double angle, u_comp, v_comp;
    };

    const std::vector<Test> test_data = {
            {225, 10, 10},
            {264.29, 1000, 100},
            {189.46, -100, -600},
            {270, 0.514444, 0}
    };

    for (const auto &test : test_data) {
        double expected = test.angle;
        double output = gribParse::calcAngle(test.u_comp, test.v_comp);
        EXPECT_NEAR(expected, output, 0.1)
                            << "Test calcMagnitude(" << test.u_comp << ", " << test.v_comp << ") failed." << std::endl;
    }
    // 0 division 0 case
    EXPECT_EQ(true,isnan(gribParse::calcAngle(0,0)));
}
