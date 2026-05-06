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
    virtual void ApplyAllNewtonIntegration( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys, double deltaTime ) const = 0;
    virtual void ApplyAllKineticEnergy( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const = 0;

    virtual std::unique_ptr<IPropertyCalculus> clone() const = 0;
};

class ClassicPropCalc : public IPropertyCalculus 
{
public:
    void ApplyAllAcceleration( const std::vector<ClassicEntity>& AllEntitys, std::vector<ClassicEntityState>& outPropertys ) const override;
    void ApplyAllNewtonIntegration( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys, double deltaTime ) const override;
    void ApplyAllKineticEnergy( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const override;

    virtual std::unique_ptr<IPropertyCalculus> clone() const override { return std::make_unique<ClassicPropCalc>(*this); }
public:
    ClassicPropCalc( std::unique_ptr<IDGLSolver> solver ) : m_DGLSolver(std::move(solver)) {}
    ClassicPropCalc( const ClassicPropCalc& other ) : m_DGLSolver(other.m_DGLSolver->clone()) {};
    ClassicPropCalc( ClassicPropCalc&& other ) : m_DGLSolver(std::move(other.m_DGLSolver)) {}
    ~ClassicPropCalc() = default;
private:
    std::unique_ptr<IDGLSolver> m_DGLSolver;
};
}
