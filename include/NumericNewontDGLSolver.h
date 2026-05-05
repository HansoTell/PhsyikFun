#pragma once

#include "Entity.h"
#include "Vector.h"
namespace Physik 
{

class IDGLSolver 
{
public:
    ~IDGLSolver() = default;
    virtual Vec3D CalcNewVelocity( const ClassicEntityState& state, double deltaTime ) const = 0;
    virtual Vec3D CalcNewPosition( const ClassicEntityState& state, double deltaTime ) const = 0;
};

class EulerCauchy : public IDGLSolver 
{
public:
    Vec3D CalcNewVelocity( const ClassicEntityState& state, double deltaTime ) const;
    Vec3D CalcNewPosition( const ClassicEntityState& state, double deltaTime ) const;
public:
    EulerCauchy() {}
    EulerCauchy( const EulerCauchy& other ) = default;
    EulerCauchy( EulerCauchy&& other ) = default;
    ~EulerCauchy() = default;
};

class VelocityVerleit : IDGLSolver 
{
public:
    Vec3D CalcNewVelocity(  const ClassicEntityState& state, double deltaTime ) const;
    Vec3D CalcNewPosition( const ClassicEntityState& state, double deltaTime ) const;
public:
    VelocityVerleit() {}
    VelocityVerleit( const VelocityVerleit& other ) = default;
    VelocityVerleit( VelocityVerleit&& other ) = default;
    ~VelocityVerleit() = default;
};

}
