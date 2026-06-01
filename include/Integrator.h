#pragma once

#include "Entity.h"
#include "Evaluator.h"
#include <memory>
#include <vector>

namespace Physik 
{
using SimulationState = std::vector<ClassicEntity>;
class IDGLSolver 
{
public:
    virtual ~IDGLSolver() = default;
    virtual void step( const SimulationState& current, SimulationState& next, std::shared_ptr<const IAccelerationEveluater> evaluator, double Time, double dt ) const = 0;

    virtual std::unique_ptr<IDGLSolver> clone() const = 0;
};

class EulerCauchy : public IDGLSolver 
{
public:
    void step( const SimulationState& current, SimulationState& next, std::shared_ptr<const IAccelerationEveluater> evaluator, double Time, double dt ) const override;

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
    void step( const SimulationState& current, SimulationState& next, std::shared_ptr<const IAccelerationEveluater> evaluator, double Time, double dt ) const override;

    std::unique_ptr<IDGLSolver> clone() const override { return std::make_unique<VelocityVerleit>(); }
public:
    VelocityVerleit() {}
    VelocityVerleit( const VelocityVerleit& other ) = default;
    VelocityVerleit( VelocityVerleit&& other ) = default;
    ~VelocityVerleit() = default;
private:
    mutable std::vector<Vec3D> m_AccScratch;
};

}
