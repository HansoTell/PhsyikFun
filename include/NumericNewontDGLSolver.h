#pragma once

#include "Entity.h"
#include "Vector.h"
#include <memory>
namespace Physik 
{

class IDGLSolver 
{
public:
    ~IDGLSolver() = default;
    virtual Vec3D CalcNewVelocity( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const = 0;
    virtual Vec3D CalcNewPosition( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const = 0;

    virtual std::unique_ptr<IDGLSolver> clone() const = 0;
};

class EulerCauchy : public IDGLSolver 
{
public:
    Vec3D CalcNewVelocity( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const override;
    Vec3D CalcNewPosition( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const override;
    std::unique_ptr<IDGLSolver> clone() const override { return std::make_unique<EulerCauchy>(); }
public:
    EulerCauchy() {}
    EulerCauchy( const EulerCauchy& other ) = default;
    EulerCauchy( EulerCauchy&& other ) = default;
    ~EulerCauchy() = default;
};

class VelocityVerleit : public IDGLSolver 
{
public:
    Vec3D CalcNewVelocity( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const override;
    Vec3D CalcNewPosition( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const override;
    std::unique_ptr<IDGLSolver> clone() const override { return std::make_unique<VelocityVerleit>(); }
public:
    VelocityVerleit() {}
    VelocityVerleit( const VelocityVerleit& other ) = default;
    VelocityVerleit( VelocityVerleit&& other ) = default;
    ~VelocityVerleit() = default;
};

}
