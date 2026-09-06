#pragma once
#include <cstddef>
#include <variant>

#include "Vector.h"

namespace Physik 
{

template <typename T = double>
struct Sphere
{
    T m_Radius;
};

template <size_t Dim = 3, typename T = double>
struct Plane 
{
    Vector<Dim, T> normal;
};

template <size_t Dim = 3, typename T = double>
struct Box 
{
    Vector<Dim, T> halfSize;
};

template <size_t Dim = 3, typename T = double>
using Shape = std::variant<
    Sphere<T>, 
    Plane<Dim, T>,
    Box<Dim, T>
>;

}
