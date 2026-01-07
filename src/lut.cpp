#include "lut.h"
bool IsSolid(int8_t type) {
    return !(
        type == 0 ||
        type == 6 ||
        type == 8 ||
        type == 9 ||
        type == 10 ||
        type == 11 ||
        type == 30 ||
        type == 31 ||
        type == 32 ||
        type == 37 ||
        type == 38 ||
        type == 39 ||
        type == 40 ||
        type == 50 ||
        type == 51 ||
        type == 55 ||
        type == 59 ||
        type == 63 ||
        type == 65 ||
        type == 66 ||
        type == 68 ||
        type == 69 ||
        type == 70 ||
        type == 72 ||
        type == 75 ||
        type == 76 ||
        type == 77 ||
        type == 78 ||
        type == 83 ||
        type == 85 ||
        type == 90 ||
        type == 92 ||
        type == 93 ||
        type == 94 ||
        type == 96
    );
}