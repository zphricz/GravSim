#ifndef VEC2_H
#define VEC2_H

class Vec2 {
    public:
        double x;
        double y;

        Vec2();
        Vec2(double x, double y);
        ~Vec2();
        
        double magnitude();
        double magnitude_square();
        double angle();
        void zero();

        Vec2& operator+=(const Vec2& rhs);
        Vec2& operator-=(const Vec2& rhs);
        Vec2& operator*=(const double factor);
        Vec2& operator=(const Vec2& rhs);
        Vec2 operator+(const Vec2& rhs);
        Vec2 operator-(const Vec2& rhs);
        Vec2 operator*(const double factor);
};

#endif
