#pragma once
#include <cstdint>
#include <ostream>
#include <sstream>

template<typename T>
std::string to_string(const T& value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

struct Byte2 {
    int8_t x,y;
    friend std::ostream& operator<<(std::ostream& os, const Byte2& a) {
        os << "(" << a.x << ", " << a.y << ")";
        return os;
    }
};

struct Byte3 {
    int8_t x,y,z;
    friend std::ostream& operator<<(std::ostream& os, const Byte3& a) {
        os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
        return os;
    }
};

struct UByte3 {
    uint8_t x,y,z;
    friend std::ostream& operator<<(std::ostream& os, const UByte3& a) {
        os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
        return os;
    }
};

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
    friend Double3 operator-(const Double3& a, const Double3& b) {
        Double3 r;
        r.x = a.x - b.x;
        r.y = a.y - b.y;
        r.z = a.z - b.z;
        return r;
    }
};