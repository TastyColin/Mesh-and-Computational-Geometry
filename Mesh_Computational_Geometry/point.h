#ifndef POINT_H
#define POINT_H
#include <math.h>

class Point
{
protected:
    double _coord[3];
public:
    Point();
    Point(float x_, float y_, float z_);
    // get
    inline double x() const {return _coord[0]; }
    inline double y() const {return _coord[1]; }
    inline double z() const {return _coord[2]; }
    double operator[](int i) const;
    double &operator[](int i);

    // calcul
    double computeNorm(void) const;
    friend double dot(const Point&, const Point&);
    friend Point prod(const Point&, const Point&);

    // Surcharge des opérateurs
    Point& operator+=(const Point&);
    Point& operator-=(const Point&);
    Point& operator/=(const double&);
    Point& operator*=(const double&);

    friend Point operator*(const double&, const Point&);
    friend Point operator/(const Point&, const double&);

};
#endif // POINT_H

Point operator+(const Point&, const Point&);
Point operator-(const Point&, const Point&);


