#pragma once

#include "Vector.h"
#include "Shapes.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <variant>

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
class EntityState 
{
public:
    KinematicState<Dim, T> m_KinState;
    EnergyPropertys<T> m_Energys;
    ConstantPrtopertys<T> m_Constants;

public:
    EntityState( Vector<Dim, T> position, Vector<Dim, T> velocity, T Mass, T Radius ) 
        : m_Constants({ Mass, 1/Mass }), m_KinState( { position, velocity, Vector<Dim, T>() } ) {}
    EntityState( const EntityState& other )  
        : m_Constants(other.m_Constants), m_KinState(other.m_KinState), m_Energys(other.m_Energys) {} 
    EntityState( EntityState&& other ) 
        : m_Constants(std::move(other.m_Constants)), m_Energys(std::move(other.m_Energys)), m_KinState(std::move(other.m_KinState)) {} 
    ~EntityState() = default;
    EntityState<Dim, T>& operator=( EntityState<Dim, T>&& other ) noexcept
    {
        if( this == &other)
            return *this;

        m_Constants = std::move(other.m_Constants);
        m_KinState = std::move(other.m_KinState);
        m_Energys = std::move(other.m_Energys);

        return *this;
    }
    EntityState<Dim, T>& operator=( const EntityState<Dim, T>& other )
    {
        if( this == &other)
            return *this;

        m_Constants = other.m_Constants;
        m_Energys = other.m_Energys;
        m_KinState = other.m_KinState;

        return *this;
    }
};

using ClassicEntityState = EntityState<3, double>;

template <size_t Dim = 3, typename T = double>
class Entity 
{
public:
    Vector<Dim, T> getPosition() const { return m_KinState.m_Position; }
    Vector<Dim, T> getVelocity() const { return m_KinState.m_Velocity; }
    Vector<Dim, T> getAcceleration() const { return m_KinState.m_Acceleration; }
    T getMass() const { return m_Constants.m_Mass; }
    T getEnergy() const { return m_Energy.KineticEnergy + m_Energy.PotentialEnergy; }
    const Shape<Dim, T>& getShape() const { return m_Shape; }
    uint64_t getID() const { return m_ID; }

    void setVelocity( const Vector<Dim, T>& newVelocity ) { m_KinState.m_Velocity = newVelocity; }
    void setPosition( const Vector<Dim, T>& newPosition ) { m_KinState.m_Position = newPosition; }
    void setAcceleration( const Vector<Dim, T>& newAcceleration ) { m_KinState.m_Acceleration = newAcceleration; }
    void setMass( T newMass ){ if(newMass == 0.0 ) return; m_Constants.m_Mass = newMass; m_Constants.m_inverseMass = 1/newMass; }
    void setKineticEnergy( T newEKin ) { m_Energy.KineticEnergy = newEKin; } 
    void setPotentialEnergy( T newEPot ) { m_Energy.PotentialEnergy = newEPot; }

    bool operator == ( const Entity& other ){ return m_ID == other.m_ID; }
public:
    Entity(Vector<Dim, T> startPosition, T mass, Shape<Dim, T> shape ) 
        : m_Constants({ mass, 1/mass }), m_KinState( { startPosition, Vector<Dim, T>(), Vector<Dim, T>() } ), m_Shape(std::move(shape)), m_ID(nextID++) {}
    Entity( Vector<Dim, T> startPosition, Vector<Dim, T> startVelocity, T mass, Shape<Dim, T> shape ) 
        : m_Constants({ mass, 1/mass }), m_KinState( { startPosition, startVelocity, Vector<Dim, T>() } ), m_Shape(std::move(shape)), m_ID(nextID++) {}
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
