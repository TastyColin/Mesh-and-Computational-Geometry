#include "point.h"

Point::Point()
{
    for (int k = 0; k < 3; k++)
    {
        _coord[k] = 0;
    }
}


Point::Point(float x_, float y_, float z_)
{
    _coord[0] = x_;
    _coord[1] = y_;
    _coord[2] = z_;
}

double Point::operator[](int k) const
{
    return _coord[k];
}

double &Point::operator[](int k)
{
    return _coord[k];
}

// Opération sur les points / vecteurs
double Point::computeNorm() const
{
    double result = 0;
    for (int k = 0; k < 3; k++)
    {
        result += _coord[k]*_coord[k];
    }
    return sqrt(result);
}

double dot(const Point& P1, const Point& P2)
{
    double result = 0;
    for (int k = 0; k < 3; k++)
    {
        result += P1._coord[k]*P2._coord[k];
    }
    return result;
}

Point prod(const Point& P1, const Point& P2)
{
    Point res;
    for (int k = 0; k < 3; k++)
    {
        res[k] = P1._coord[(k+1)%3]*P2._coord[(k+2)%3] - P1._coord[(k+2)%3]*P2._coord[(k+1)%3];
    }
    return res;
}

// Surcharge des opérateurs
Point& Point::operator+=(const Point& other)
{
    for (int k = 0; k < 3; k++)
    {
        _coord[k] += other._coord[k];
    }
    return *this;
}

Point& Point::operator-=(const Point& other)
{
    for (int k = 0; k < 3; k++)
    {
        _coord[k] -= other._coord[k];
    }
    return *this;
}

Point& Point::operator*=(const double& d)
{
    for (int k = 0; k < 3; k++)
    {
        _coord[k] *= d;
    }
    return *this;
}

Point& Point::operator/=(const double& d)
{
    for (int k = 0; k < 3; k++)
    {
        _coord[k] /= d;
    }
    return *this;
}


Point operator+(const Point& P1, const Point& P2)
{
    Point result = P1;
    return result += P2;
}

Point operator-(const Point& P1, const Point& P2)
{
    Point result = P1;
    return result -= P2;
}

Point operator*(const double& d, const Point& P)
{
    Point result = P;
    result *= d;
    return result;
}

Point operator/(const Point& P, const double& d)
{
    Point result = P;
    result /= d;
    return result;
}









