#ifndef GRADIENT_H
#define GRADIENT_H


class Gradient
{
public:
    Gradient();

    static double red(double x);
    static double green(double x);
    static double blue(double x);


protected:
    static const double _C_NEGATIF[3];
    static const double _C_NULL[3];
    static const double _C_POSITIF[3];
};


class GradientRayon : public Gradient
{
public:
    GradientRayon();
    static double red(double x);
    static double green(double x);
    static double blue(double x);

};


#endif // GRADIENT_H
