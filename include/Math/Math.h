#pragma once

#include "Vector.h"
#include <cstddef>

namespace Math
{
namespace VectorCalc 
{
using ::Physik::Vector;

template <size_t Dim = 3, typename T = double>
Vector<Dim, T> VectorNormal( const Vector<Dim, T>& first, const Vector<Dim, T>& secound )
{
    Vector<Dim, T> diff = secound - first;

    return diff / diff.EukNorm();
}
template <size_t Dim = 3, typename T = double>
T VectorProduct( const Vector<Dim, T>& first, const Vector<Dim, T>& secound )
{
    T erg = 0;
    for( size_t i = 0; i < Dim; i++ )
    {
        erg += first[i] * secound[i];
    }
    return erg;
}

template <typename T = double>
Vector<3, T> CrossProduct( const Vector<3, T>& first, const Vector<3, T>& secound )
{
    Vector<3, T> erg;
    erg[0] = first[1] * secound[2] - first[2] * secound[1];
    erg[1] = first[2] * secound[0] - first[0] * secound[2];
    erg[2] = first[0] * secound[1] - first[1] * secound[0];

    return erg;
}
}
} 
