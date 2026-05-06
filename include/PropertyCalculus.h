#pragma once

#include "Entity.h"
#include "NumericNewontDGLSolver.h"
#include <memory>
#include <vector>

namespace Physik 
{

class IPropertyCalculus 
{
public:
    ~IPropertyCalculus() = default;
    virtual void ApplyAllAcceleration( const std::vector<ClassicEntity>& AllEntitys, std::vector<ClassicEntityState>& outPropertys ) const = 0;
    virtual void ApplyAllNewtonIntegration( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const = 0;
    virtual void ApplyAllKineticEnergy( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const = 0;
};

class ClassicPropCalc : public IPropertyCalculus 
{
public:
    void ApplyAllAcceleration( const std::vector<ClassicEntity>& AllEntitys, std::vector<ClassicEntityState>& outPropertys ) const override;
    void ApplyAllNewtonIntegration( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const override;
    void ApplyAllKineticEnergy( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const override;
public:
    ClassicPropCalc( std::unique_ptr<IDGLSolver> solver ) : m_DGLSolver(std::move(solver)) {}
    //TODO:
    ClassicPropCalc( const ClassicPropCalc& other ) {};
    ClassicPropCalc( ClassicPropCalc&& other ) : m_DGLSolver(std::move(other.m_DGLSolver)) {}
    ~ClassicPropCalc() = default;
private:
    std::unique_ptr<IDGLSolver> m_DGLSolver;
};
}
