#ifndef __P2POINT_H__
#define __P2POINT_H__

#include <math.h>

// Base class p2Point
template<class TYPE>
class p2Point
{
public:
    TYPE x, y;

    // Default constructor
    p2Point() : x(0), y(0) {}

    // Constructor taking x and y
    p2Point(TYPE x, TYPE y) : x(x), y(y) {}

    // Copy constructor
    p2Point(const p2Point& p) : x(p.x), y(p.y) {}

    // Destructor
    virtual ~p2Point() {}
};

// Derived class p2Vector
template<class TYPE>
class p2Vector : public p2Point<TYPE>
{
public:
    // Default constructor
    p2Vector() : p2Point<TYPE>() {}

    // Constructor taking another p2Point
    p2Vector(const p2Point<TYPE>& p) : p2Point<TYPE>(p) {}

    // Constructor taking x and y
    p2Vector(TYPE x, TYPE y) : p2Point<TYPE>(x, y) {}

    void Normalize()
    {
        float m = this->x * this->x + this->y * this->y;
        if (m > 0.0f)
        {
            float inv_m = 1 / sqrtf(m);
            this->x *= inv_m;
            this->y *= inv_m;
        }
    }

    float Length() const
    {
        return sqrtf(float((this->x * this->x) + (this->y * this->y)));
    }

    p2Vector operator -(const p2Vector<TYPE>& v) const
    {
        p2Vector<TYPE> r;

        r.x = this->x - v.x;
        r.y = this->y - v.y;

        return r;
    }

    p2Vector& operator*=(float a)
    {
        this->x *= a;
        this->y *= a;
        return *this;
    }

    p2Vector operator*(float a) const
    {
        p2Vector<TYPE> r;

        r.x = this->x * a;
        r.y = this->y * a;

        return r;
    }

    p2Vector& operator/=(float a)
    {
        float f = 1.0f / a;
        this->x *= f;
        this->y *= f;
        return *this;
    }

    TYPE dot(const p2Vector<TYPE>& v) const
    {
        return (this->x * v.x + this->y * v.y);
    }
};

// Aliases for specific types
typedef p2Point<int> iPoint;
typedef p2Point<float> fPoint;
typedef p2Vector<int> iVector;
typedef p2Vector<float> fVector;

#endif // __P2POINT_H__
