#pragma once

#include "Vector.h"
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
    T M_Radius;
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
        : m_Constants({ Mass, Radius }), m_KinState( { position, velocity, Vector<Dim, T>() } ) {}
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
    Vector<Dim, T> getPosition() const { return m_State.m_KinState.m_Position; }
    Vector<Dim, T> getVelocity() const { return m_State.m_KinState.m_Velocity; }
    Vector<Dim, T> getAcceleration() const { return m_State.m_KinState.m_Acceleration; }
    T getMass() const { return m_State.m_Constants.m_Mass; }
    T getRadius() const { return m_State.m_Constants.M_Radius; }
    const EntityState<Dim, T>& getEntityState() const { return m_State; }
    EntityState<Dim, T> getEntityStateCopy() const { return m_State; }
    T getEnergy() const { return m_State.m_Energys.KineticEnergy + m_State.m_Energys.PotentialEnergy; }
    uint64_t getID() const { return m_ID; }

    void setVelocity( const Vector<Dim, T>& newVelocity ) { m_State.m_KinState.m_Velocity = newVelocity; }
    void setPosition( const Vector<Dim, T>& newPosition ) { m_State.m_KinState.m_Position = newPosition; }
    void setAcceleration( const Vector<Dim, T>& newAcceleration ) { m_State.m_KinState.m_Acceleration = newAcceleration; }
    void setMass( T newMass ){ m_State.m_Constants.m_Mass = newMass; }
    void setKineticEnergy( T newEKin ) { m_State.m_Energys.KineticEnergy = newEKin; } 
    void setPotentialEnergy( T newEPot ) { m_State.m_Energys.PotentialEnergy = newEPot; }
public:
    Entity(){}
    Entity(Vector<Dim, T> startPosition, T mass ) : m_State( { startPosition, Vector<Dim, T>(), mass, 1.0 } ), m_ID(nextID++) {}
    Entity(Vector<Dim, T> startPosition, T mass, T Radius ) : m_State( { startPosition, Vector<Dim, T>(), mass, Radius } ), m_ID(nextID++) {}
    Entity( Vector<Dim, T> startPosition, Vector<Dim, T> startVelocity, T mass, T Radius ) : m_State( { startPosition, std::move(startVelocity),  mass, Radius } ), m_ID(nextID++) {}
    Entity( const Entity<Dim, T>& other ) : m_State( other.m_State ), m_ID(other.m_ID) {}
    Entity( Entity<Dim, T>&& other ) : m_State( std::move(other.m_State) ), m_ID(other.m_ID){ other.m_ID = 0; }
    ~Entity() = default;
private:
    inline static uint64_t nextID = 1;
private:
    uint64_t m_ID;
    EntityState<Dim, T> m_State;
};

using ClassicEntity = Entity<3, double>;
    
}
