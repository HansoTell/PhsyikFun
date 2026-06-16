#pragma once

#include "Vector.h"

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
}
} 
