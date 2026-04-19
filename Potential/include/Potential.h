#pragma once

#include "Vector.h"
#include <cstddef>

namespace Physik 
{
class IPotential 
{
public:
    ~IPotential() = default;
    virtual double getForce( double distance ) const = 0;
    virtual double getPotentialEnergy( double distance ) const = 0;
};


//wie richtig machen dass man nicht ditance rein geben muss sondern ein vector damit man zwischen den beidne die distanc rechnen kann --> zudem was mit einer methode um minus von vecktoren zur echnen ohne die zu verändern sondern der zurück gibt als ergbenis
template <size_t Dim>
class ClassicalPotential : public IPotential 
{
public:
    double getForce( double distance ) const override;
    double getPotentialEnergy( double distance ) const override;
public:
    ClassicalPotential( double beta, Vector<Dim> position );
    ClassicalPotential( const ClassicalPotential& other ) = delete;
    ClassicalPotential( ClassicalPotential&& other ) = delete;
    ~ClassicalPotential() = default;
private:
    Vector<Dim> m_Position;
};
}
