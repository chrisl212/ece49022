#include "math.h"

float atan(float z) {
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;
    return (n1 + n2 * z * z) * z;
}

float fabsf(float x) {
    return (x > 0.0) ? x : -x;
}

float atan2(float y, float x) {
    if (x != 0.0f) {
        if (fabsf(x) > fabsf(y)) {
            const float z = y / x;
            if (x > 0.0) {
                return atan(z);
            } else if (y >= 0.0) {
                return atan(z) + PI;
            } else {
                return atan(z) - PI;
            }
        } else {
            const float z = x / y;
            if (y > 0.0) {
                return -atan(z) + PI_2;
            } else {
                return -atan(z) - PI_2;
            }
        }
    } else {
        if (y > 0.0f) {
            return PI_2;
        } else if (y < 0.0f) {
            return -PI_2;
        }
    }
    return 0.0f; 
}

int pow(int a, int b) {
    int res = 1;
    while (b--) {
        res *= a;
    }
    return res;
}

