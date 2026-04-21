#pragma once

#include "Vector.h"
#include <cstddef>

//brauchen modul das sich um das versetzten von teilchen und berechnen davon kümmert...
namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class IPotential 
{
public:
    ~IPotential() = default;
    virtual Vector<Dim, T> getForce( Vector<Dim, T> positionEntity, double time ) const = 0;
    virtual T getPotentialEnergy( Vector<Dim, T> positionEntity, double time ) const = 0;
};


template <size_t Dim = 3, typename T = double>
class StandartPotential : public IPotential<Dim, T> 
{
public:
    Vector<Dim, T> getForce( Vector<Dim, T> positionEntity, double time ) const override
    {
        T distance = positionEntity.EukNorm();
        double constant = - m_Beta  / (distance*distance*distance);

    }
    T getPotentialEnergy( Vector<Dim, T> positionEntity, double time ) const override { return - m_Beta / positionEntity.EukNorm(); }
public:
    StandartPotential( T beta, Vector<Dim, T> position ) : m_Beta(beta), m_Position(std::move(position)){}
    StandartPotential( const StandartPotential& other ) = delete;
    StandartPotential( StandartPotential&& other ) = delete;
    ~StandartPotential() = default;
private:
    T m_Beta;
    Vector<Dim, T> m_Position;
};

using ClassicPotential = IPotential<3, double>;
using ClassicStandartPotential = StandartPotential<3, double>;
}
