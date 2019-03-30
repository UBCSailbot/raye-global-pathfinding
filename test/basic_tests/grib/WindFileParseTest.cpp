// Copyright 2017 UBC Sailbot

#include "WindFileParseTest.h"
#include "grib/windFileParse.h"
#include <eccodes.h>

WindGribParseTest::WindGribParseTest() {}

TEST_F(WindGribParseTest, TestMagnitude) {
    struct Test {
        double knot_output,u_comp, v_comp;
    };

    const std::vector<Test> test_data = {
            {0, 0, 0},
            {27.549,10,10}
    };

    for (const auto &test : test_data) {
        double expected = test.knot_output;
        double output = WindFileParse::calcMagnitude(test.u_comp, test.v_comp);
        EXPECT_NEAR(expected, output, 0.1)
                            << "Test calcMagnitude(" << test.u_comp << ", " << test.v_comp << ") failed." << std::endl;
    }
}
