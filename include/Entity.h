#pragma once

#include "Vector.h"
#include "Potential.h"
#include <cstddef>
#include <memory>
#include <vector>

#define CREATE_POSITON_VEC3D(x, y, z) std::make_shared<Vec3D>(Vec3D { x, y, z} )

namespace Physik 
{

template <size_t Dim = 3, typename T = double>
class Entity 
{
public:
    Vector<Dim, T> getPosition() const { return *m_Position; }
    Vector<Dim, T> getVelocity() const { return m_Velocity; }
    T getMass() const { return m_Mass; }
    const std::vector<std::unique_ptr<IPotential<Dim, T>>>& getIntrinsicPotentials() const { return m_OwnPotentials; }

    void setVelocity( const Vector<Dim, T>& newVelocity ) { m_Velocity = newVelocity; }
    void setPosition( const Vector<Dim, T>& newPosition ) { *m_Position = newPosition; }
    void setMass( T newMass ){ m_Mass = newMass; }
    void addIntrinsivPotential( std::unique_ptr<IPotential<Dim, T>> newPotential ){ m_OwnPotentials.push_back(std::move(newPotential)); }
public:
    Entity(){}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, T mass ) : m_Position(startPosition), m_Mass(mass){}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, Vector<Dim, T> startVelocity ) : m_Position(startPosition), m_Velocity(std::move(startVelocity)) {}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, T mass, Vector<Dim, T> startVelocity ) : m_Position(startPosition), m_Velocity(std::move(startVelocity)), m_Mass(mass) {}
    Entity( const Entity<Dim, T>& other ) : 
        m_Mass(other.m_Mass), 
        m_Velocity(other.m_Velocity), 
        m_Position(std::make_shared<Vec3D>(*other.m_Position)) 
    {
        m_OwnPotentials.reserve(other.m_OwnPotentials.size());
        for( int i = 0; i < other.m_OwnPotentials.size(); i++ )
        {
            m_OwnPotentials.push_back(other.m_OwnPotentials[i]->clone());
        }
    }
    Entity( Entity<Dim, T>&& other ) : 
        m_Mass(other.m_Mass), 
        m_Velocity(std::move(other.m_Velocity)), 
        m_Position(other.m_Position), 
        m_OwnPotentials(std::move(other.m_OwnPotentials))
    {
        other.m_Mass = 0.0;
        other.m_Velocity.fill(0.0);
        other.m_Position = nullptr;
    }
    ~Entity() = default;
private:
    std::shared_ptr<Vector<Dim, T>> m_Position;
    Vector<Dim, T> m_Velocity;
    T m_Mass;

    std::vector<std::unique_ptr<IPotential<Dim, T>>> m_OwnPotentials;
};

using ClassicEntity = Entity<3, double>;
    
}
