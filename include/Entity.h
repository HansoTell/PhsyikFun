#pragma once

#include "Vector.h"
#include "Potential.h"
#include <cstddef>
#include <memory>
#include <vector>

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
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, T mass ) : m_Position(startPosition), m_Mass(mass){}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, Vector<Dim, T> startVelocity ) : m_Position(startPosition), m_Velocity(std::move(startVelocity)) {}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, T mass, Vector<Dim, T> startVelocity ) : m_Position(startPosition), m_Velocity(std::move(startVelocity)), m_Mass(mass) {}
    Entity( const Entity<Dim, T>& other ) = default;
    Entity( Entity<Dim, T>&& other ) = default;
    ~Entity() = default;
private:
    std::shared_ptr<Vector<Dim, T>> m_Position;
    Vector<Dim, T> m_Velocity;
    T m_Mass;

    std::vector<std::unique_ptr<ClassicPotential>> m_OwnPotentials;
};

using ClassicEntity = Entity<3, double>;
    
}
