#include "NumericNewontDGLSolver.h"
#include "Vector.h"

namespace Physik 
{
Vec3D EulerCauchy::CalcNewVelocity( const ClassicEntityState& state, double deltaTime ) const
{
    Vec3D newVelocity = state.m_Velocity + deltaTime * state.m_Acceleration;
    return newVelocity;
}
Vec3D EulerCauchy::CalcNewPosition( const ClassicEntityState& state, double deltaTime ) const
{
    Vec3D newPos = *state.m_Position + ( deltaTime * state.m_Velocity );
    return newPos;
}

Vec3D VelocityVerleit::CalcNewVelocity( const ClassicEntityState& state, double deltaTime ) const
{

}
Vec3D VelocityVerleit::CalcNewPosition( const ClassicEntityState& state, double deltaTime ) const
{
 //   Vec3D newPos = oldPosition + Velocity*deltaTime ;//+ 0.5 *Acceleration * deltatime*deltatime

}
}
