#pragma once

#include "Entity.h"
#include "Potential.h"
#include "Vector.h"
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
    //TODO
    Field( const Field& other ) = default;
    Field ( Field&& other ) = default;
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
    //TODO
    Interaction( const Interaction& other ) = default;
    Interaction( Interaction&& other ) = default;
    ~Interaction() = default;
private:
    std::unique_ptr<IPotential<Dim, T>> m_Potential;
};

using ClassicField = Field<3, double>;
using ClassicInteraction = Interaction<3, double>;
}
