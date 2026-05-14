#pragma once

#include "Vector.h"
#include <cstddef>


namespace Physik 
{

template <size_t Dim = 3, typename T = double> 
struct EntityState 
{
    Vector<Dim, T> m_Position;
    Vector<Dim, T> m_Velocity;
    Vector<Dim, T> m_Acceleration;
    Vector<Dim, T> m_Force;
    T m_Mass;
    T KineticEnergy;
    T PotentialEnergy;


public:
    EntityState( Vector<Dim, T> position, Vector<Dim, T> velocity, T Mass ) 
        : m_Mass(Mass), m_Position(position), m_Velocity(velocity) {}
    EntityState( const EntityState& other )  
        : m_Mass(other.m_Mass), m_Velocity(other.m_Velocity), m_Position(other.m_Position), 
        m_Force(other.m_Force), m_Acceleration(other.m_Acceleration), KineticEnergy(other.KineticEnergy), PotentialEnergy(other.PotentialEnergy) {}
    EntityState( EntityState&& other ) : 
        m_Position(std::move(other.m_Position)), m_Velocity(std::move(other.m_Velocity)), m_Acceleration(std::move(other.m_Acceleration)),
        m_Mass(std::move(other.m_Mass)), m_Force(std::move(other.m_Force)), KineticEnergy(std::move(other.KineticEnergy)), PotentialEnergy(std::move(other.PotentialEnergy)) {}
    ~EntityState() = default;
    EntityState<Dim, T>& operator=( EntityState<Dim, T>&& other ) noexcept
    {
        if( this == &other)
            return *this;

        m_Mass = std::move(other.m_Mass);
        m_Velocity = std::move(other.m_Velocity);
        m_Position = std::move(other.m_Position);
        m_Force = std::move(other.m_Force);
        m_Acceleration = std::move(other.m_Acceleration);
        KineticEnergy = std::move(other.KineticEnergy);
        PotentialEnergy = std::move(other.PotentialEnergy);

        return *this;
    }
    EntityState<Dim, T>& operator=( const EntityState<Dim, T>& other )
    {
        if( this == &other)
            return *this;
        m_Mass = other.m_Mass;
        m_Velocity = other.m_Velocity;
        m_Position = other.m_Position;
        m_Force = other.m_Force;
        m_Acceleration = other.m_Acceleration;
        KineticEnergy = other.KineticEnergy;
        PotentialEnergy = other.PotentialEnergy;

        return *this;
    }
};

using ClassicEntityState = EntityState<3, double>;

template <size_t Dim = 3, typename T = double>
class Entity 
{
public:
    Vector<Dim, T> getPosition() const { return m_State.m_Position; }
    Vector<Dim, T> getVelocity() const { return m_State.m_Velocity; }
    Vector<Dim, T> getAcceleration() const { return m_State.m_Acceleration; }
    Vector<Dim, T> getForce() const { return m_State.m_Force; }
    T getMass() const { return m_State.m_Mass; }
    const EntityState<Dim, T>& getEntityState() const { return m_State; }
    EntityState<Dim, T> getEntityStateCopy() const { return m_State; }
    T getEnergy() const { return m_State.KineticEnergy + m_State.PotentialEnergy; }

    void setVelocity( const Vector<Dim, T>& newVelocity ) { m_State.m_Velocity = newVelocity; }
    void setPosition( const Vector<Dim, T>& newPosition ) { m_State.m_Position = newPosition; }
    void setMass( T newMass ){ m_State.m_Mass = newMass; }
    void setKineticEnergy( T newEKin ) { m_State.KineticEnergy = newEKin; } 
    void setPotentialEnergy( T newEPot ) { m_State.PotentialEnergy = newEPot; }

    void setEntityState( EntityState<Dim, T> NewEntityState ){ m_State = NewEntityState; }
public:
    Entity(){}
    Entity(Vector<Dim, T> startPosition, T mass ) : m_State( { startPosition, Vector<Dim, T>(), mass } ){}
    Entity( Vector<Dim, T> startPosition, Vector<Dim, T> startVelocity, T mass ) : m_State( { startPosition, std::move(startVelocity),  mass } ) {}
    Entity( const Entity<Dim, T>& other ) : m_State( other.m_State ) {}
    Entity( Entity<Dim, T>&& other ) : m_State( std::move(other.m_State) ){}
    ~Entity() = default;
private:
    EntityState<Dim, T> m_State;
};

using ClassicEntity = Entity<3, double>;
    
}
