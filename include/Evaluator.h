#pragma once

#include "Vector.h"
#include "Entity.h"
#include "Interactions.h"

#include <vector>

namespace Physik 
{
using SimulationState = std::vector<ClassicEntity>;
class IAccelerationEveluater 
{
public:
    virtual ~IAccelerationEveluater() = default;

    virtual void CalcAccelerations( const SimulationState& state, double Time, std::vector<Vec3D>& outAccelerations ) const  = 0;
    virtual void CalcPotEnergy( const SimulationState& state, double Time, std::vector<double>& outEpots ) const = 0;
    virtual void CalcKineticEnergy( const SimulationState& state, std::vector<double>& outEKins ) const = 0;

    virtual void UpdateAccelerations( SimulationState& state, double Time ) = 0;
    virtual void UpdateKineticEnergys( SimulationState& state ) = 0;
    virtual void UpdatePotentialEnergys( SimulationState& state, double Time ) = 0;

    virtual void addExternPotential( ClassicField potential ) = 0;
    virtual void addMulitpleExternPotentials( std::vector<ClassicField> potentials ) = 0;
    virtual void addEntityPotential( ClassicInteraction potential) = 0;
    virtual void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) = 0;

    virtual void Clear() = 0;
    virtual std::shared_ptr<IAccelerationEveluater> clone() const = 0;
};


class WorldEvaluator : public IAccelerationEveluater 
{
public:
    void CalcAccelerations( const SimulationState& state, double Time, std::vector<Vec3D>& outAccelerations ) const override;
    void CalcPotEnergy( const SimulationState& state, double Time, std::vector<double>& outEpots ) const override;
    void CalcKineticEnergy( const SimulationState& state, std::vector<double>& outEKins ) const override;

    void UpdateAccelerations( SimulationState& state, double Time ) override;
    void UpdateKineticEnergys( SimulationState& state ) override;
    void UpdatePotentialEnergys( SimulationState& state, double Time ) override;

    void addExternPotential( ClassicField potential ) override;
    void addMulitpleExternPotentials( std::vector<ClassicField> potentials ) override;
    void addEntityPotential( ClassicInteraction potential) override;
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) override;

    void Clear() override { m_EntityPotentials.clear(); m_ExtPotentials.clear(); }
    std::shared_ptr<IAccelerationEveluater> clone() const override { return std::make_shared<WorldEvaluator>(*this); }
public:
    WorldEvaluator();
    WorldEvaluator( const WorldEvaluator& other );
    WorldEvaluator( WorldEvaluator&& other );
    ~WorldEvaluator() = default;
private:
    std::vector<ClassicField> m_ExtPotentials;
    std::vector<ClassicInteraction> m_EntityPotentials;

    mutable std::vector<Vec3D> m_AccScratch;
    mutable std::vector<double> m_EkinScratch;
    mutable std::vector<double> m_EPotScratch;
};
}
