#pragma once

#include "NumericNewontDGLSolver.h"

#include <memory>

namespace Physik 
{

class IPropertyCalculus 
{
public:
    ~IPropertyCalculus() = default;
    //wie bennen das es möglichst sinn macht?? --> oder doch in 2 methoden splitten?
    virtual void CalcForceandAcceleration() const = 0;
    virtual void CalcPositonandVelocity() const = 0;
    virtual void CalcNewEnergy() const = 0;
};

class ClassicPropCalc : public IPropertyCalculus 
{
public:
    void CalcForceandAcceleration() const;
    void CalcPositonandVelocity() const;
    void CalcNewEnergy() const;
public:
    ClassicPropCalc( std::unique_ptr<IDGLSolver> solver ) : m_DGLSolver(std::move(solver)) {}
    //TODO:
    ClassicPropCalc( const ClassicPropCalc& other ) {};
    ClassicPropCalc( ClassicPropCalc&& other ) : m_DGLSolver(std::move(other.m_DGLSolver)) {}
    ~ClassicPropCalc() = default;
private:
    double CalcPotnetialEnergy() const;
    double CalcKineticEnergy() const;
private:
    std::unique_ptr<IDGLSolver> m_DGLSolver;
};
}
