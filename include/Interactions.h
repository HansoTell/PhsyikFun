#pragma once

#include "Entity.h"
#include "Potential.h"
#include "Vector.h"
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>

namespace Physik 
{
template<size_t Dim = 3, typename T = double>
class Field 
{
public:
    Vector<Dim, T> getForce( const EntityState<Dim, T>& entity, double time ) const { return m_Potential->getForce(entity, m_State, time); }
    T getPotentialEnergy( const EntityState<Dim, T>& entity, double time ) const { return m_Potential->getPotentialEnergy(entity, m_State, time); }
public:
    Field( std::unique_ptr<IPotential<Dim, T>> potential, EntityState<Dim, T> state ) : m_Potential(std::move(potential)), m_State(std::move(state)) {}
    Field( const Field& other ) : m_Potential(other.m_Potential->clone()), m_State(other.m_State) {}
    Field ( Field&& other ) : m_Potential(std::move(other.m_Potential)), m_State(std::move(other.m_State)) {} 
    ~Field() = default;
private:
    std::unique_ptr<IPotential<Dim, T>> m_Potential;
    EntityState<Dim, T> m_State;
};

template<size_t Dim = 3, typename T = double>
class Interaction 
{
public:
    Vector<Dim, T> getForce( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const { return m_Potential->getForce(ent1, ent2, time); }
    T getPotentialEnergy( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const { return m_Potential->getPotentialEnergy(ent1, ent2, time); }
public:
    Interaction( std::unique_ptr<IPotential<Dim, T>> potential ) : m_Potential(std::move(potential)) {}
    Interaction( const Interaction& other ) : m_Potential(other.m_Potential->clone()) {} 
    Interaction( Interaction&& other ) : m_Potential(std::move(other.m_Potential)){}
    ~Interaction() = default;
private:
    std::unique_ptr<IPotential<Dim, T>> m_Potential;
};

template<size_t Dim = 3, typename T = double>
class NonPotentialForce 
{
public:
    Vector<Dim, T> getForce( const EntityState<Dim, T>& entity, double time ) const { return m_ForceCalcer(entity, time); }
public:
    NonPotentialForce( std::function<Vector<Dim, T>( const EntityState<Dim, T>& entity, double time )> force ) : m_ForceCalcer(std::move(force)) {}
    NonPotentialForce( const NonPotentialForce& other ) : m_ForceCalcer(other.m_ForceCalcer) {}
    NonPotentialForce( NonPotentialForce&& other ) : m_ForceCalcer(std::move(other.m_ForceCalcer)) {}
    ~NonPotentialForce() = default;
private:
    std::function<Vector<Dim, T>( const EntityState<Dim, T>& entity, double time )> m_ForceCalcer;
};

using ClassicField = Field<3, double>;
using ClassicInteraction = Interaction<3, double>;
using ClassicNonPotentialForce = NonPotentialForce<3, double>;
}
