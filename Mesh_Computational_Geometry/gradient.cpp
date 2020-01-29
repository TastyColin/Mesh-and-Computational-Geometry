#include "gradient.h"

const double Gradient::_C_NEGATIF[3] = {1., 0., 0.5};
const double Gradient::_C_NULL[3] = {1., 0.8, 0.4};
const double Gradient::_C_POSITIF[3] = {0., 0.5, 1.};

Gradient::Gradient() {}

double Gradient::red(double x)
{
    int i_c = 0;
    if (x < 0.5) { return 2*x*_C_NULL[i_c] + (1-2*x)*_C_NEGATIF[i_c]; }
    else { return (2-2*x)*_C_NULL[i_c] + (2*x-1)*_C_POSITIF[i_c]; }
}

double Gradient::green(double x)
{
    int i_c = 1;
    if (x < 0.5) { return 2*x*_C_NULL[i_c] + (1-2*x)*_C_NEGATIF[i_c]; }
    else { return (2-2*x)*_C_NULL[i_c] + (2*x-1)*_C_POSITIF[i_c]; }
}

double Gradient::blue(double x)
{
    int i_c = 2;
    if (x < 0.5) { return 2*x*_C_NULL[i_c] + (1-2*x)*_C_NEGATIF[i_c]; }
    else { return (2-2*x)*_C_NULL[i_c] + (2*x-1)*_C_POSITIF[i_c]; }
}


// Gradient Rayon

GradientRayon::GradientRayon() {}

double GradientRayon::red(double x)
{
    int i_c = 0;
    if (x < 0.5) { return 2*x*_C_NEGATIF[i_c] + (1-2*x)*_C_NULL[i_c]; }
    else { return (2-2*x)*_C_POSITIF[i_c] + (2*x-1)*_C_NULL[i_c]; }
}

double GradientRayon::green(double x)
{
    int i_c = 1;
    if (x < 0.5) { return 2*x*_C_NEGATIF[i_c] + (1-2*x)*_C_NULL[i_c]; }
    else { return (2-2*x)*_C_POSITIF[i_c] + (2*x-1)*_C_NULL[i_c]; }
}

double GradientRayon::blue(double x)
{
    int i_c = 2;
    if (x < 0.5) { return 2*x*_C_NEGATIF[i_c] + (1-2*x)*_C_NULL[i_c]; }
    else { return (2-2*x)*_C_POSITIF[i_c] + (2*x-1)*_C_NULL[i_c]; }
}
