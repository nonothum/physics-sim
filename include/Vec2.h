#pragma once
#include <cmath>

struct Vec2 {
    float x = 0.f, y = 0.f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(float s)        const { return {x * s,   y * s};   }
    Vec2 operator/(float s)        const { return {x / s,   y / s};   }

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(float s)       { x *= s;   y *= s;   return *this; }

    float length() const { return std::sqrt(x*x + y*y); }
    float lengthSq() const { return x*x + y*y; }
    float dot(const Vec2& o) const { return x*o.x + y*o.y; }

    Vec2 normalized() const {
        float len = length();
        return (len > 1e-6f) ? Vec2{x/len, y/len} : Vec2{0, 0};
    }
};
