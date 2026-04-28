#pragma once

#include "Vector.h"
#include <cstddef>
#include <memory>


#include <iostream>

namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class IPotential 
{
public:
    ~IPotential() = default;
    virtual Vector<Dim, T> getForce( const Vector<Dim, T>& positionEntity, T mass, double time ) const = 0;
    virtual T getPotentialEnergy( const Vector<Dim, T>& positionEntity, T mass, double time ) const = 0;
    virtual std::unique_ptr<IPotential> clone() const = 0;
};


template <size_t Dim = 3, typename T = double>
class StandartPotential : public IPotential<Dim, T> 
{
public:
    //distance muss riochtig gemacht werden wenn distance --> 0 geht was dann??? Kann nicht einfach error machen
    Vector<Dim, T> getForce( const Vector<Dim, T>& positionEntity, T mass, double time ) const override
    {
        Vector<Dim, T> offset = positionEntity - (*m_Position);
        T distance = offset.EukNorm();

        double constant = - m_Beta / (distance*distance*distance);

        return offset * constant;
    }
    T getPotentialEnergy( const Vector<Dim, T>& positionEntity, T mass, double time ) const override 
    {
        Vector<Dim, T> offset = positionEntity - (*m_Position);

        return - m_Beta / offset.EukNorm(); 
    }
    std::unique_ptr<IPotential<Dim, T>> clone() const override { return std::make_unique<StandartPotential>(*this); }
public:
    StandartPotential( T beta, std::shared_ptr<Vector<Dim, T>> position ) : m_Beta(beta), m_Position(position){}
    StandartPotential( const StandartPotential& other ) = default ;
    StandartPotential( StandartPotential&& other ) = default;
    ~StandartPotential() = default;
private:
    T m_Beta;
    std::shared_ptr<Vector<Dim, T>> m_Position;
};

using ClassicPotential = IPotential<3, double>;
using ClassicStandartPotential = StandartPotential<3, double>;
}
