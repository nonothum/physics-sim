#include "Vec2.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace {
bool approx(float a, float b, float eps = 1e-5f)
{
    return std::fabs(a - b) <= eps;
}

void test_default_constructor()
{
    Vec2 v;

    assert(approx(v.x, 0.f));
    assert(approx(v.y, 0.f));
}

void test_value_constructor()
{
    Vec2 v{2.5f, -4.f};

    assert(approx(v.x, 2.5f));
    assert(approx(v.y, -4.f));
}

void test_length()
{
    Vec2 v{3.f, 4.f};

    assert(approx(v.length(), 5.f));
    assert(approx(v.lengthSq(), 25.f));
}

void test_dot()
{
    Vec2 a{1.f, 3.f};
    Vec2 b{2.f, -5.f};

    assert(approx(a.dot(b), -13.f));
}

void test_normalized()
{
    Vec2 v{3.f, 4.f};
    Vec2 n = v.normalized();

    assert(approx(n.x, 0.6f));
    assert(approx(n.y, 0.8f));
    assert(approx(n.length(), 1.f));
}

void test_zero_normalized()
{
    Vec2 zero;
    Vec2 n = zero.normalized();

    assert(approx(n.x, 0.f));
    assert(approx(n.y, 0.f));
}
}

int main()
{
    test_default_constructor();
    test_value_constructor();
    test_length();
    test_dot();
    test_normalized();
    test_zero_normalized();

    return 0;
}
