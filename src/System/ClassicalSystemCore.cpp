#include "Entity.h"
#include "Interactions.h"
#include "SystemCore.h"

#include <cassert>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

namespace Physik 
{
ClassicalSystemCore::ClassicalSystemCore()
    : m_DeltaTime(default_delta_time), m_Integrator(std::make_unique<VelocityVerleit>()), 
    m_Evaluater(std::make_shared<WorldEvaluator>()), m_Impact(std::make_unique<AdvancedElasticImpact>()),
    m_Time(0.0), m_Tmax(std::numeric_limits<double>::infinity()) 
{}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod ) 
    : m_DeltaTime(default_delta_time), m_Integrator(std::move(dglMethod)), 
    m_Evaluater(std::make_shared<WorldEvaluator>()), m_Impact(std::make_unique<AdvancedElasticImpact>()),
    m_Time(0.0), m_Tmax(std::numeric_limits<double>::infinity()) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod, double deltaTime ) 
    : m_DeltaTime( deltaTime ), m_Integrator(std::move(dglMethod)), m_Evaluater(std::make_shared<WorldEvaluator>()), m_Impact(std::make_unique<AdvancedElasticImpact>()), m_Time(0.0) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( const ClassicalSystemCore& other ) 
    : m_DeltaTime(other.m_DeltaTime), m_Time(other.m_Time), Energy(other.Energy),
    m_CurrentState(other.m_CurrentState),  m_Integrator(other.m_Integrator->clone()), m_Evaluater(other.m_Evaluater->clone()), m_Impact(other.m_Impact->clone()) {}

ClassicalSystemCore::ClassicalSystemCore( ClassicalSystemCore&& other ) 
    : m_DeltaTime(other.m_DeltaTime), m_Time(other.m_Time), Energy(other.Energy), 
    m_Integrator(std::move(other.m_Integrator)), 
    m_CurrentState(std::move(other.m_CurrentState)),
    m_Evaluater(std::move(other.m_Evaluater)),
    m_Impact(std::move(other.m_Impact)){}

void ClassicalSystemCore::Clear() 
{
    m_CurrentState.clear();
    m_NextState.clear();
    m_Evaluater->Clear();
    m_Time = 0.0;
    m_DeltaTime = default_delta_time;
    Energy = 0.0; 
}

void ClassicalSystemCore::addEntity( ClassicEntity entity ) 
{ 
    ClassicEntity EntityCopy = entity;
    m_NextState.add(std::move(EntityCopy));
    m_CurrentState.add(std::move(entity));
}
bool ClassicalSystemCore::addEntity( Vec3D startPosition, Vec3D startVelocity, double mass, double Radius )
{
    ClassicEntity Entity(std::move(startPosition), std::move(startVelocity), mass, Radius);
    ClassicEntity EntityCopy = Entity;

    return  m_CurrentState.add(std::move(Entity)) && m_NextState.add(std::move(EntityCopy));
}

void ClassicalSystemCore::addMulipleEntitys( std::vector<ClassicEntity> entitys ) 
{ 
    for( int i = 0; i < entitys.size(); i++ )
    {
        ClassicEntity EntCopy = entitys[i];
        m_NextState.add(std::move(EntCopy));
        m_CurrentState.add(std::move(entitys[i]));
    } 
}
void ClassicalSystemCore::addExternPotential( ClassicField extPotential ) { m_Evaluater->addExternPotential(std::move(extPotential)); }
void ClassicalSystemCore::addMulitpleExternPotentials( std::vector<ClassicField> potentials ) { m_Evaluater->addMulitpleExternPotentials(std::move(potentials)); }
void ClassicalSystemCore::addEntityPotential( ClassicInteraction potential ) { m_Evaluater->addEntityPotential(std::move(potential)); }
void ClassicalSystemCore::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) { m_Evaluater->addMultipleEntityPotentials(std::move(potentials)); }
void ClassicalSystemCore::addNonPotentialForce( ClassicNonPotentialForce NonPotForce ) { m_Evaluater->addNonPotentialForce(std::move(NonPotForce)); }
void ClassicalSystemCore::addMultipleNonPotentialForce( std::vector<ClassicNonPotentialForce> NonPotForce ) { m_Evaluater->addMulitpleNonPotentialForce(std::move(NonPotForce)); }

void ClassicalSystemCore::Step()
{
    m_Integrator->step(m_CurrentState, m_NextState, m_Evaluater, m_Time, m_DeltaTime);
    m_Impact->ApplyImpacts(m_NextState);

    advanceTimeIncrement();
    
    m_Evaluater->UpdateAccelerations(m_NextState, m_Time);
    m_Evaluater->UpdateKineticEnergys(m_NextState);
    m_Evaluater->UpdatePotentialEnergys(m_NextState, m_Time);

    std::swap(m_NextState, m_CurrentState);

}

void ClassicalSystemCore::UpdateEntityPropertys()
{
    m_Evaluater->UpdateAccelerations(m_CurrentState, m_Time);
    m_Evaluater->UpdateKineticEnergys(m_CurrentState);
    m_Evaluater->UpdatePotentialEnergys(m_CurrentState, m_Time);
}

void ClassicalSystemCore::advanceTimeIncrement() { m_Time += m_DeltaTime; }

}
