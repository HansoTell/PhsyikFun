#pragma once

#include "Interactions.h"
#include "Entity.h"
#include "PropertyCalculus.h"
#include <memory>
#include <vector>

namespace Physik 
{
using SimulationState = std::vector<ClassicEntity>;

class IAccelerationEveluater 
{
public:
    virtual ~IAccelerationEveluater() = 0;

    virtual std::vector<Vec3D> evaluate( const SimulationState& state ) const = 0;

    virtual void addExternPotential( ClassicField potential ) = 0;
    virtual void addMulitpleExternPotentials( std::vector<ClassicField> potentials ) = 0;
    virtual void addEntityPotential( ClassicInteraction potential) = 0;
    virtual void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) = 0;
};


class WorldEvaluator : public IAccelerationEveluater 
{
public:
    std::vector<Vec3D> evaluate( const SimulationState& state ) const override;

    void addExternPotential( ClassicField potential ) override;
    void addMulitpleExternPotentials( std::vector<ClassicField> potentials ) override;
    void addEntityPotential( ClassicInteraction potential) override;
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) override;
public:
    WorldEvaluator();
    WorldEvaluator( const WorldEvaluator& other );
    WorldEvaluator( WorldEvaluator&& other );
    ~WorldEvaluator() = default;
private:
    std::vector<ClassicField> m_ExtPotentials;
    std::vector<ClassicInteraction> m_EntityPotentials;
};


class ClassicalSystemCore 
{
    static constexpr double default_delta_time = 0.001;
public:
    void Clear();
    void addExternPotential( ClassicField potential );
    void addMulitpleExternPotentials( std::vector<ClassicField> potentials );
    void addEntity( ClassicEntity entity );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );
    void setTimeIncrement( double DeltaTime ) { m_DeltaTime = DeltaTime; }
    void addEntityPotential( ClassicInteraction potential);
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials );

    void UpdateEntityPropertys();
    void UpdateEnergy();

    void MakeTimeStep();

    const std::vector<ClassicEntity>& getEntitys() const { return m_CurrentState; }
    double getEnergy() const { return Energy; }
    double getTime() const { return m_Time; }

public:
    ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> PropertyCalcer );
    ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> dglMethod, double DeltaTime );
    ClassicalSystemCore(const ClassicalSystemCore& other);
    ClassicalSystemCore( ClassicalSystemCore&& other);
    ~ClassicalSystemCore() = default;
private:
    void advanceTimeIncrement();
    std::vector<ClassicEntityState> ClacNewEntityStates() const;
    void ApplyAllForces( std::vector<ClassicEntityState>& outPropertys ) const;
    void ApplyAllEnergy( std::vector<ClassicEntityState>& outPropertys ) const;
    void ApplyNewEntityStates( const std::vector<ClassicEntityState>& Propertys );
private:
    SimulationState m_CurrentState;
    SimulationState m_NextState;

    std::vector<ClassicField> m_ExtPotentials;
    std::vector<ClassicInteraction> m_EntityPotentials;

    double m_Time;
    double m_DeltaTime;
    double Energy;
    
    std::unique_ptr<IPropertyCalculus> m_PropertyCalcer;
};
}
