#pragma once

#include "Vector.h"
#include <cstddef>

namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class Entity 
{
public:
    Vector<Dim, T> getPosition() const { return m_Position; }
    Vector<Dim, T> getVelocity() const { return m_Velocity; }
    T getMass() const { return m_Mass; }


private:
    Vector<Dim, T> m_Position;
    Vector<Dim, T> m_Velocity;
    T m_Mass;
};
    
}
