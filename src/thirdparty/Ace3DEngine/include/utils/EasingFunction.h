//
// Created by begosshu on 2020/9/8.
//

#ifndef TNT_EASINGFUNCTION_H
#define TNT_EASINGFUNCTION_H

namespace utils {

enum EasingFunctionType
{
    EasingFunctionTypeLinear = 0,
    EasingFunctionTypeEaseInQuad = 1,
    EasingFunctionTypeEaseOutQuad = 2,
    EasingFunctionTypeEaseInOutQuad = 3,

    EasingFunctionTypeEaseInCubic = 4,
    EasingFunctionTypeEaseOutCubic = 5,
    EasingFunctionTypeEaseInOutCubic = 6,

    EasingFunctionTypeEaseInQuart = 7,
    EasingFunctionTypeEaseOutQuart = 8,
    EasingFunctionTypeEaseInOutQuart = 9,

    EasingFunctionTypeEaseInBounce = 10,
    EasingFunctionTypeEaseOutBounce = 11,

    EasingFunctionTypeEaseInExpo = 12,
    EasingFunctionTypeEaseOutExpo = 13,
    EasingFunctionTypeEaseInOutExpo = 14
};

float easingAnimation(float percent, EasingFunctionType type);

}

#endif //TNT_EASINGFUNCTION_H
