#pragma once

#include "Vector.h"
#include <cstddef>

namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class IPotential 
{
public:
    ~IPotential() = default;
    virtual Vector<Dim, T> getForce( Vector<Dim, T> positionEntity ) const = 0;
    virtual Vector<Dim, T> getPotentialEnergy( Vector<Dim, T> positionEntity ) const = 0;
};


template <size_t Dim = 3, typename T = double>
class ClassicalPotential : public IPotential<Dim, T> 
{
public:
    Vector<Dim, T> getForce( Vector<Dim, T> positionEntity ) const override;
    Vector<Dim, T> getPotentialEnergy( Vector<Dim, T> positionEntity ) const override;
public:
    ClassicalPotential( double beta, Vector<Dim, T> position );
    ClassicalPotential( const ClassicalPotential& other ) = delete;
    ClassicalPotential( ClassicalPotential&& other ) = delete;
    ~ClassicalPotential() = default;
private:
    Vector<Dim, T> m_Position;
};
}
