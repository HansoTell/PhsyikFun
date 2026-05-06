#pragma once

#include "Vector.h"
#include <cstddef>
#include <memory>

#define CREATE_POSITON_VEC3D(x, y, z) std::make_shared<Vec3D>(Vec3D { x, y, z} )

namespace Physik 
{

//TODO: Konstruktor und Makros
//TODO: schauen wie wir es am performantestenm mit der position pointer machen kann ja nicht sein dass da dauerhaft heaped wird
template <size_t Dim = 3, typename T = double> 
struct EntityState 
{
    std::shared_ptr<Vector<Dim, T>> m_Position;
    Vector<Dim, T> m_Velocity;
    Vector<Dim, T> m_Acceleration;
    Vector<Dim, T> m_Force;
    T m_Mass;
    T KineticEnergy;
    T PotentialEnergy;
};

using ClassicEntityState = EntityState<3, double>;

template <size_t Dim = 3, typename T = double>
class Entity 
{
public:
    Vector<Dim, T> getPosition() const { return *m_State.m_Position; }
    Vector<Dim, T> getVelocity() const { return m_State.m_Velocity; }
    Vector<Dim, T> getAcceleration() const { return m_State.m_Acceleration; }
    T getMass() const { return m_State.m_Mass; }
    const EntityState<Dim, T>& getEntityState() const { return m_State; }
    EntityState<Dim, T> getEntityStateCopy() const { return m_State; }
    T getEnergy() const { return m_State.KineticEnergy + m_State.PotentialEnergy; }

    void setVelocity( const Vector<Dim, T>& newVelocity ) { m_State.m_Velocity = newVelocity; }
    void setPosition( const Vector<Dim, T>& newPosition ) { *m_State.m_Position = newPosition; }
    void setMass( T newMass ){ m_State.m_Mass = newMass; }
    void setKineticEnergy( T newEKin ) { m_State.KineticEnergy = newEKin; } 
    void setPotentialEnergy( T newEPot ) { m_State.PotentialEnergy = newEPot; }

    void setEntityState( const EntityState<Dim, T>& NewEntityState ){ m_State = NewEntityState; }
public:
    //TODO: Constructors
    Entity(){}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, T mass ) : m_State( { startPosition, Vector<Dim, T>(), mass} ){}
    Entity( std::shared_ptr<Vector<Dim, T>> startPosition, Vector<Dim, T> startVelocity, T mass ) : m_State( { startPosition, std::move(startVelocity), Vector<Dim, T>(), Vector<Dim, T>(), mass, 0.5*mass* startVelocity.EukNorm() * startVelocity.EukNorm(), 0.0 } ) {}
    Entity( const Entity<Dim, T>& other ) : 
         m_State( { std::make_shared<Vector<Dim, T>>(*other.m_State.m_Position), other.m_State.m_Velocity, other.m_State.m_Acceleration, other.m_State.m_Force, other.m_State.m_Mass, other.m_State.KineticEnergy, other.m_State.PotentialEnergy } ) {}
    Entity( Entity<Dim, T>&& other ) : 
        m_State( other.m_State )
    {
        other.m_State.m_Mass = 0.0;
        other.m_State.m_Velocity.fill(0.0);
        other.m_State.m_Position = nullptr;
    }
    ~Entity() = default;
private:
    EntityState<Dim, T> m_State;
};

using ClassicEntity = Entity<3, double>;
    
}
