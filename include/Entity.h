#pragma once

#include "Vector.h"
#include "Shapes.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace Physik 
{
template <size_t Dim = 3, typename T = double> 
struct KinematicState 
{
    Vector<Dim, T> m_Position;
    Vector<Dim, T> m_Velocity;
    Vector<Dim, T> m_Acceleration;
};

template <typename T = double>
struct EnergyPropertys 
{
    T KineticEnergy;
    T PotentialEnergy;
};

template <typename T = double>
struct ConstantPrtopertys
{
    T m_Mass;
    T m_inverseMass;
};

template <size_t Dim = 3, typename T = double>
class Entity 
{
public:
    Vector<Dim, T> getPosition() const { return m_KinState.m_Position; }
    Vector<Dim, T> getVelocity() const { return m_KinState.m_Velocity; }
    Vector<Dim, T> getAcceleration() const { return m_KinState.m_Acceleration; }
    T getMass() const { return m_Constants.m_Mass; }
    T getInverseMass() const { return m_Constants.m_inverseMass; }
    bool isStatic() const { return m_Constants.m_inverseMass == T{0}; }
    T getEnergy() const { return getKineticEnergy() + getPotentialEnergy(); }
    T getKineticEnergy() const { return m_Energy.KineticEnergy; }
    T getPotentialEnergy() const { return m_Energy.PotentialEnergy; }
    const Shape<Dim, T>& getShape() const { return m_Shape; }
    uint64_t getID() const { return m_ID; }

    void setVelocity( const Vector<Dim, T>& newVelocity ) { m_KinState.m_Velocity = newVelocity; }
    void setPosition( const Vector<Dim, T>& newPosition ) { m_KinState.m_Position = newPosition; }
    void setAcceleration( const Vector<Dim, T>& newAcceleration ) { m_KinState.m_Acceleration = newAcceleration; }
    void setMass( T newMass ){ if(newMass == T{0} ) return; m_Constants.m_Mass = newMass; m_Constants.m_inverseMass = 1/newMass; }
    void setKineticEnergy( T newEKin ) { m_Energy.KineticEnergy = newEKin; } 
    void setPotentialEnergy( T newEPot ) { m_Energy.PotentialEnergy = newEPot; }

    bool operator == ( const Entity& other ){ return m_ID == other.m_ID; }
public:
    Entity(Vector<Dim, T> startPosition, T mass, Shape<Dim, T> shape, bool isStatic = false ) 
        : m_KinState( { startPosition, Vector<Dim, T>(), Vector<Dim, T>() } ), m_Energy({ 0.0, 0.0 }), m_Shape(std::move(shape)), m_ID(nextID++) 
    {
        if( isStatic || mass == 0 )
            m_Constants = { mass, 0 };
        else
            m_Constants = { mass, 1/mass };
    }
    Entity( Vector<Dim, T> startPosition, Vector<Dim, T> startVelocity, T mass, Shape<Dim, T> shape, bool isStatic = false ) 
        : m_Constants({ mass, 1/mass }), m_KinState( { startPosition, startVelocity, Vector<Dim, T>() } ), m_Energy({ 0.5 * mass * startVelocity * startVelocity, 0.0 }), m_Shape(std::move(shape)), m_ID(nextID++) 
    {
        if( isStatic || mass == T{0} )
            m_Constants = { mass, T{0} };
        else
            m_Constants = { mass, 1/mass };
    }
    Entity( const Entity<Dim, T>& other ) : m_KinState(other.m_KinState), m_Energy(other.m_Energy), m_Constants(other.m_Constants), m_Shape(other.m_Shape), m_ID(other.m_ID) {}
    Entity( Entity<Dim, T>&& other ) : m_KinState(std::move(other.m_KinState)), m_Energy(std::move(other.m_Energy)), m_Constants(std::move(other.m_Constants)), m_Shape(std::move(other.m_Shape)), m_ID(other.m_ID){ other.m_ID = 0; }
    ~Entity() = default;
    Entity& operator=(const Entity&) = default;
    Entity& operator=(Entity&&) = default;
private:
    inline static uint64_t nextID = 1;
private:
    uint64_t m_ID;
    KinematicState<Dim, T> m_KinState;
    EnergyPropertys<T> m_Energy;
    ConstantPrtopertys<T> m_Constants;

    Shape<Dim, T> m_Shape;
};

using ClassicEntity = Entity<3, double>;
    
}
