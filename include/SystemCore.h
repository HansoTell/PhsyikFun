#pragma once

#include "Interactions.h"
#include "Entity.h"
#include "Integrator.h"
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

    void addEntity( ClassicEntity entity );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );

    void setTimeIncrement( double DeltaTime ) { m_DeltaTime = DeltaTime; }

    void UpdateEntityPropertys();
    void Step();

    const std::vector<ClassicEntity>& getEntitys() const { return m_CurrentState; }
    double getEnergy() const { return Energy; }
    double getTime() const { return m_Time; }

public:
    ClassicalSystemCore( std::unique_ptr<IDGLSolver> PropertyCalcer );
    ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod, double DeltaTime );
    ClassicalSystemCore(const ClassicalSystemCore& other);
    ClassicalSystemCore( ClassicalSystemCore&& other);
    ~ClassicalSystemCore() = default;
private:
    void advanceTimeIncrement();
private:
    SimulationState m_CurrentState;
    SimulationState m_NextState;

    double m_Time;
    double m_DeltaTime;
    double Energy;
    
    std::unique_ptr<IDGLSolver> m_Integrator;
    std::shared_ptr<IAccelerationEveluater> m_Evaluater;
};
}
