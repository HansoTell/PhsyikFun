#pragma once

#include "EntityRegistry.h"
#include "Interactions.h"
#include "Entity.h"
#include "Integrator.h"
#include "Impact.h"
#include "Vector.h"

#include <memory>
#include <vector>

namespace Physik 
{
using SimulationState = std::vector<ClassicEntity>;

class ClassicalSystemCore 
{
    static constexpr double default_delta_time = 0.001;
public:
    void Clear();
    void addExternPotential( ClassicField potential );
    void addMulitpleExternPotentials( std::vector<ClassicField> potentials );

    void addEntityPotential( ClassicInteraction potential);
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials );

    void addNonPotentialForce( ClassicNonPotentialForce NonPotForce );
    void addMultipleNonPotentialForce( std::vector<ClassicNonPotentialForce> NonPotForce );

    void addEntity( ClassicEntity entity );
    bool addEntity( Vec3D startPosition, Vec3D startVelocity, double mass, double Radius );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );

    void setTimeIncrement( double DeltaTime ) { m_DeltaTime = DeltaTime; }
    void setTmax( double Tmax ) { m_Tmax = Tmax; }

    void UpdateEntityPropertys();
    void Step();

    const EntityRegistry& getEntityRegister() const { return m_CurrentState; }
    double getEnergy() const { return Energy; }
    double getTime() const { return m_Time; }
    double getTmax() const { return m_Tmax; }

public:
    ClassicalSystemCore();
    ClassicalSystemCore( std::unique_ptr<IDGLSolver> PropertyCalcer );
    ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod, double DeltaTime );
    ClassicalSystemCore( const ClassicalSystemCore& other );
    ClassicalSystemCore( ClassicalSystemCore&& other);
    ~ClassicalSystemCore() = default;
private:
    void advanceTimeIncrement();
private:
    EntityRegistry m_CurrentState;
    EntityRegistry m_NextState;

    double m_Time;
    double m_Tmax;
    double m_DeltaTime;
    double Energy;
    
    std::unique_ptr<IDGLSolver> m_Integrator;
    std::unique_ptr<IImpactEvaluator> m_Impact;
    std::shared_ptr<IAccelerationEveluater> m_Evaluater;
};
}
