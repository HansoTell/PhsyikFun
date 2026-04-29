#pragma once

#include "Constants.h"
#include "Entity.h"
#include "Vector.h"
#include <cstddef>
#include <memory>


namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class IPotential 
{
public:
    ~IPotential() = default;
    virtual Vector<Dim, T> getForce( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const = 0;
    virtual T getPotentialEnergy( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const = 0;
    virtual std::unique_ptr<IPotential> clone() const = 0;
};


template <size_t Dim = 3, typename T = double>
class StandartPotential : public IPotential<Dim, T> 
{
public:
    //distance muss riochtig gemacht werden wenn distance --> 0 geht was dann??? Kann nicht einfach error machen
    Vector<Dim, T> getForce( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const override
    {
        Vector<Dim, T> offset = *ent1.m_Position - *ent2.m_Position;
        T distance = offset.EukNorm();

        double constant = - m_Beta / (distance*distance*distance);

        return offset * constant;
    }
    T getPotentialEnergy( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const override 
    {
        Vector<Dim, T> offset = *ent1.m_Position - *ent2.m_Position;

        return - m_Beta / offset.EukNorm(); 
    }
    std::unique_ptr<IPotential<Dim, T>> clone() const override { return std::make_unique<StandartPotential>(*this); }
public:
    StandartPotential( T beta ) : m_Beta(beta) {}
    StandartPotential( const StandartPotential& other ) = default ;
    StandartPotential( StandartPotential&& other ) = default;
    ~StandartPotential() = default;
private:
    T m_Beta;
};

template <size_t Dim = 3, typename T = double>
class GravitationalPotential : public IPotential<Dim, T> 
{
public:
    Vector<Dim, T> getForce( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const override
    {
        Vector<Dim, T> offset = *ent1.m_Position - *ent2.m_Position;
        double constant  =  GravtationalKonstant * ent1.m_Mass * ent2.m_Mass /(offset.EukNorm() * offset.EukNorm() * offset.EukNorm());
        return offset * constant;
    }
    T getPotentialEnergy( const EntityState<Dim, T>& ent1, const EntityState<Dim, T>& ent2, double time ) const override
    {
        Vector<Dim, T> offset = *ent1.m_Position - *ent2.m_Position;

        return - GravtationalKonstant * ent1.m_Mass * ent2.m_Mass / offset.EukNorm();
    }
    std::unique_ptr<IPotential<Dim, T>> clone() const override { return std::make_unique<GravitationalPotential>(); }
public:
    GravitationalPotential() {}
    GravitationalPotential( const GravitationalPotential& other ) = default;
    GravitationalPotential( GravitationalPotential&& other ) = default;
    ~GravitationalPotential() = default;
};

using ClassicIPotential = IPotential<3, double>;
using ClassicStandartPotential = StandartPotential<3, double>;
using ClassicGravitationPotential = GravitationalPotential<3, double>;
}
