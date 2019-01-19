#include <iostream>
#include "StandardCalc.h"
using namespace std;

double StandardCalc::BoundTo180(double angle){
    while (angle > 180 || angle < -180) {
        if (angle > 180)
            angle -= 360;
        else if
                (angle < -180)
            angle += 360;
    }
    return angle;
}

bool StandardCalc::IsAngleBetween(float first_angle, float middle_angle, float second_angle) {
    bool inBetween = false;
    float upperBound;
    float lowerBound;

    //assign larger num to upperBound, smaller num to lowerBound
    if (first_angle > second_angle) {
        upperBound = first_angle;
        lowerBound = second_angle;
    }
    else {
        upperBound = second_angle;
        lowerBound = first_angle;
    }

    /* Case 1: The upperBound has a difference than less than 180 compared to
     *         the lowerBound.
     */
    if ((upperBound - lowerBound) < 180 ) {
        if ((middle_angle > lowerBound) && (middle_angle < upperBound))
            inBetween = true;
    }
    /* Case 2: The difference between the upperBound and the lowerBound
     *         is 180. In this case, every angle between the the UpperBound
     *         and LowerBound is inBetween except when the angle is equal to
     *         UpperBound or LowerBound
     */
    else if (upperBound - lowerBound == 180){
        if (middle_angle != upperBound || middle_angle != lowerBound)
            inBetween = true;
    }

    /* Case 3: The difference between the upperBound and the lowerBound is larger
     *         than 180 degrees. This would generally cause the acute angle to between the
     *         2nd and 3rd quadrant of the circle.
     */
    else {
        if ((lowerBound > middle_angle && middle_angle >= -180) || (upperBound < middle_angle && middle_angle <= 180))
            inBetween = true;
    }

    return inBetween;
}
