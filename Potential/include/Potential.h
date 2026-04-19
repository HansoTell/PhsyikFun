#pragma once

#include "Vector.h"

namespace Physik 
{
class IPotential 
{
public:
    ~IPotential() = default;
    virtual double getForce( double distance ) const = 0;
    virtual double getPotentialEnergy( double distance ) const = 0;
};


//wie mit dimensionene machen?? templaten wir einfach alles durch? oder machen wir interfacve klasse für die dimensionen die gebraucht werden?
class StandartPotential : public IPotential 
{
public:
    double getForce( double distance ) const override;
    double getPotentialEnergy( double distance ) const override;
public:
    StandartPotential( double beta, Vector<3> );
    StandartPotential( const StandartPotential& other ) = delete;
    StandartPotential( StandartPotential&& other ) = delete;
    ~StandartPotential() = default;
private:
    Vector<3> m_Position;
};
}
