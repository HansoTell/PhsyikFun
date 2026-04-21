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
public:
    Entity();
    Entity( Vector<Dim, T> startPosition, double mass );
    Entity( Vector<Dim, T> startPosition, Vector<Dim, T> startVelocity );
    Entity( Vector<Dim, T> startPosition, double mass, Vector<Dim, T> startVelocity );
    //other EntityKonstruxters
    Entity( const Entity<Dim, T>& other ) = default;
    Entity( Entity<Dim, T>&& other ) = default;
    ~Entity() = default;
private:
    Vector<Dim, T> m_Position;
    Vector<Dim, T> m_Velocity;
    T m_Mass;
};

using ClassicEntity = Entity<3, double>;
    
}
