#include "NumericNewontDGLSolver.h"
#include "Vector.h"

namespace Physik 
{
Vec3D EulerCauchy::CalcNewVelocity( const ClassicEntity& entity, const ClassicEntityState& newState, double deltaTime ) const
{
    Vec3D newVelocity = entity.getVelocity() + deltaTime * newState.m_Acceleration;
    return newVelocity;
}
Vec3D EulerCauchy::CalcNewPosition( const ClassicEntity& state, const ClassicEntityState& newState, double deltaTime ) const
{
    Vec3D newPos = state.getPosition() +  deltaTime * newState.m_Velocity;
    return newPos;
}

Vec3D VelocityVerleit::CalcNewVelocity( const ClassicEntity& state, const ClassicEntityState& newState, double deltaTime ) const
{
    Vec3D newVelocity = state.getVelocity() + 0.5 * ( state.getAcceleration() + newState.m_Acceleration) * deltaTime;
    return newVelocity;
}
Vec3D VelocityVerleit::CalcNewPosition( const ClassicEntity& state, const ClassicEntityState& newState, double deltaTime ) const
{
    Vec3D newPos = state.getPosition() + state.getVelocity() * deltaTime + 0.5 * state.getAcceleration() * deltaTime *  deltaTime;
    return newPos;
}
}
