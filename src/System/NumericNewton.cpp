#include "NumericNewontDGLSolver.h"
#include "Vector.h"

namespace Physik 
{
Vec3D EulerCauchy::CalcNewVelocity( Vec3D oldVelocity, Vec3D Acceleration, double deltaTime ) const
{
    return oldVelocity += Acceleration * deltaTime;
}
Vec3D EulerCauchy::CalcNewPosition( Vec3D oldPosition, Vec3D Velocity, double deltaTime ) const
{
    return oldPosition += Velocity * deltaTime;
}

}
