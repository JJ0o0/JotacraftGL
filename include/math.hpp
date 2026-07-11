#pragma once

inline int FloorDivision(int value, int divisor) {
    int result = value / divisor;
    int remainder = value % divisor;

    if (remainder != 0 && ((remainder < 0) != (divisor < 0))) --result;
    return result;
} 