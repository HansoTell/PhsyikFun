#include "Integrator.h"
#include "Vector.h"
#include <cstddef>
#include <memory>
#include <vector>

namespace Physik 
{
void EulerCauchy::step( const SimulationState& current, SimulationState& next, std::shared_ptr<const IAccelerationEveluater> evaluator, double Time, double dt ) const 
{
    for( size_t i = 0; i < next.size(); i++ )
    {
        Vec3D newVelocity = current[i].getVelocity() + dt * current[i].getAcceleration();
        next[i].setVelocity(newVelocity);
    }

    for( size_t i = 0; i < next.size(); i++ )
    {
        Vec3D newPos = current[i].getPosition() + dt * next[i].getVelocity();
        next[i].setPosition(newPos);
    }
}

void VelocityVerleit::step( const SimulationState& current, SimulationState& next, std::shared_ptr<const IAccelerationEveluater> evaluator, double Time, double dt ) const 
{
    for( size_t i = 0; i < next.size(); i++ )
    {
        Vec3D halfVelc = current[i].getVelocity() + 0.5 * current[i].getAcceleration() * dt;
        next[i].setVelocity(halfVelc);
    }

    for( size_t i = 0; i < next.size(); i++ )
    {
        Vec3D newPos = current[i].getPosition() + next[i].getVelocity() * dt;
        next[i].setPosition(newPos);
    }

    m_AccScratch.assign(next.size(), Vec3D{});
    evaluator->CalcAccelerations(next, Time+dt, m_AccScratch);
    for( size_t i = 0; i < next.size(); i++ )
    {
        Vec3D newVeloc = next[i].getVelocity() + 0.5 * m_AccScratch[i] * dt;
        next[i].setVelocity(newVeloc);
    }
}
}
