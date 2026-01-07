#pragma once
#include <cstdint>
#include <ostream>

struct Int2 {
    int32_t x,y;
    friend std::ostream& operator<<(std::ostream& os, const Int2& a) {
        os << "(" << a.x << ", " << a.y << ")";
        return os;
    }
};


struct Int3 {
    int32_t x,y,z;
    friend std::ostream& operator<<(std::ostream& os, const Int3& a) {
        os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
        return os;
    }
};


struct Float2 {
    float x,y;
    friend std::ostream& operator<<(std::ostream& os, const Float2& a) {
        os << "(" << a.x << ", " << a.y << ")";
        return os;
    }
};

struct Double2 {
    double x,y;
    friend std::ostream& operator<<(std::ostream& os, const Double2& a) {
        os << "(" << a.x << ", " << a.y << ")";
        return os;
    }
};


struct Float3 {
    float x,y,z;
    friend std::ostream& operator<<(std::ostream& os, const Float3& a) {
        os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
        return os;
    }
};

struct Double3 {
    double x,y,z;
    friend std::ostream& operator<<(std::ostream& os, const Double3& a) {
        os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
        return os;
    }
};