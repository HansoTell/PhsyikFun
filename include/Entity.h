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
    Entity(){}
    Entity( Vector<Dim, T> startPosition, T mass ) : m_Position(std::move(startPosition)), m_Mass(mass){}
    Entity( Vector<Dim, T> startPosition, Vector<Dim, T> startVelocity ) : m_Position(std::move(startPosition)), m_Velocity(std::move(startVelocity)) {}
    Entity( Vector<Dim, T> startPosition, T mass, Vector<Dim, T> startVelocity ) : m_Position(std::move(startPosition)), m_Velocity(std::move(startVelocity)), m_Mass(mass) {}
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
