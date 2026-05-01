#pragma once

#include "Vector.h"
namespace Physik 
{

class IDGLSolver 
{
public:
    ~IDGLSolver() = default;
    virtual Vec3D CalcNewVelocity( Vec3D oldVelocity, Vec3D Acceleration, double deltaTime ) const = 0;
    virtual Vec3D CalcNewPosition( Vec3D oldPosition, Vec3D Velocity, double deltaTime ) const = 0;
};

class EulerCauchy : public IDGLSolver 
{
public:
    Vec3D CalcNewVelocity( Vec3D oldVelocity, Vec3D Acceleration, double deltaTime ) const;
    Vec3D CalcNewPosition( Vec3D oldPosition, Vec3D Velocity, double deltaTime ) const;

public:
    EulerCauchy() {}
    EulerCauchy( const EulerCauchy& other ) = default;
    EulerCauchy( EulerCauchy&& other ) = default;
    ~EulerCauchy() = default;
};

class VelocityVerleit : IDGLSolver 
{

};

}
