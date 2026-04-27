#pragma once

#include "Vector.h"
#include <cstddef>
#include <memory>

//brauchen modul das sich um das versetzten von teilchen und berechnen davon kümmert...
namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class IPotential 
{
public:
    ~IPotential() = default;
    virtual Vector<Dim, T> getForce( const Vector<Dim, T>& positionEntity, T mass, double time ) const = 0;
    virtual T getPotentialEnergy( const Vector<Dim, T>& positionEntity, T mass, double time ) const = 0;
};


template <size_t Dim = 3, typename T = double>
class StandartPotential : public IPotential<Dim, T> 
{
public:
    Vector<Dim, T> getForce( const Vector<Dim, T>& positionEntity, T mass, double time ) const override
    {
    //einfach falsch ist nicht in bezug zu position des Kordinaten des potentials 
        T distance = positionEntity.EukNorm();
        //distance muss riochtig gemacht werden wenn distance --> 0 geht was dann??? Kann nicht einfach error machen
        
        double constant = - m_Beta / (distance*distance*distance);
        Vector<Dim, T> erg;
        for( int i = 0; i < Dim; i++ )
        {
            erg.at(i) = positionEntity[i] * constant;
        }

        return erg;
    }
    T getPotentialEnergy( const Vector<Dim, T>& positionEntity, T mass, double time ) const override { return - m_Beta / positionEntity.EukNorm(); }
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
