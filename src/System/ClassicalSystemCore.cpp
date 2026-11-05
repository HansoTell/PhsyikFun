#include "Entity.h"
#include "EntityDescribtion.h"
#include "EntityRegistry.h"
#include "Interactions.h"
#include "SystemCore.h"

#include <cassert>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include <chrono>

namespace Physik 
{
ClassicalSystemCore::ClassicalSystemCore()
    : m_DeltaTime(default_delta_time), m_Integrator(std::make_unique<VelocityVerleit>()), 
    m_Evaluater(std::make_shared<WorldEvaluator>()), m_DynamicEntImpact(std::make_unique<AdvancedElasticImpact>()),
    m_Time(0.0), m_Tmax(std::numeric_limits<double>::infinity()) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod ) 
    : m_DeltaTime(default_delta_time), m_Integrator(std::move(dglMethod)), 
    m_Evaluater(std::make_shared<WorldEvaluator>()), m_DynamicEntImpact(std::make_unique<AdvancedElasticImpact>()),
    m_Time(0.0), m_Tmax(std::numeric_limits<double>::infinity()) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod, double deltaTime ) 
    : m_DeltaTime( deltaTime ), m_Integrator(std::move(dglMethod)), m_Evaluater(std::make_shared<WorldEvaluator>()), m_DynamicEntImpact(std::make_unique<AdvancedElasticImpact>()), m_Time(0.0) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( const ClassicalSystemCore& other ) 
    : m_DeltaTime(other.m_DeltaTime), m_Time(other.m_Time), Energy(other.Energy),
    m_CurrentState(other.m_CurrentState),  m_Integrator(other.m_Integrator->clone()), m_Evaluater(other.m_Evaluater->clone()), m_DynamicEntImpact(other.m_DynamicEntImpact->clone()) {}

ClassicalSystemCore::ClassicalSystemCore( ClassicalSystemCore&& other ) 
    : m_DeltaTime(other.m_DeltaTime), m_Time(other.m_Time), Energy(other.Energy), 
    m_Integrator(std::move(other.m_Integrator)), 
    m_CurrentState(std::move(other.m_CurrentState)),
    m_Evaluater(std::move(other.m_Evaluater)),
    m_DynamicEntImpact(std::move(other.m_DynamicEntImpact)){}

void ClassicalSystemCore::Clear() 
{
    m_CurrentState.clear();
    m_NextState.clear();
    m_Evaluater->Clear();
    m_Time = 0.0;
    m_DeltaTime = default_delta_time;
    Energy = 0.0; 
}

EntityRegistry::ID ClassicalSystemCore::addEntity( EntityDescription desc )
{
    ClassicEntity Entity(std::move(desc));
    ClassicEntity EntityCopy = Entity;

    auto ID = Entity.getID();
    bool isStatic = Entity.isStatic();

    m_CurrentState.add(std::move(Entity));  
    m_NextState.add(std::move(EntityCopy));

    if( isStatic )
        m_StaticEntImpact->NotifyEntityAddition(m_CurrentState, m_CurrentState.getStaticEntitys());
    else
        m_DynamicEntImpact->NotifyEntityAddition( m_CurrentState, m_CurrentState.getDynamicEntitys() );

    return ID;
}

std::vector<EntityRegistry::ID> ClassicalSystemCore::addMulipleEntitys( std::vector<EntityDescription> entitys ) 
{ 
    std::vector<EntityRegistry::ID> AllIDs;
    AllIDs.reserve(entitys.size());

    for( int i = 0; i < entitys.size(); i++ )
    {
        ClassicEntity Ent(entitys[i]);
        ClassicEntity EntCopy = Ent;
        auto ID = Ent.getID();
        m_NextState.add(std::move(EntCopy));
        m_CurrentState.add(std::move(Ent));

        AllIDs.push_back(ID);
    } 

    m_DynamicEntImpact->NotifyEntityAddition( m_CurrentState, m_CurrentState.getDynamicEntitys() );
    m_StaticEntImpact->NotifyEntityAddition( m_CurrentState, m_CurrentState.getStaticEntitys() );

    return AllIDs;
}
void ClassicalSystemCore::addExternPotential( ClassicField extPotential ) { m_Evaluater->addExternPotential(std::move(extPotential)); }
void ClassicalSystemCore::addMulitpleExternPotentials( std::vector<ClassicField> potentials ) { m_Evaluater->addMulitpleExternPotentials(std::move(potentials)); }
void ClassicalSystemCore::addEntityPotential( ClassicInteraction potential ) { m_Evaluater->addEntityPotential(std::move(potential)); }
void ClassicalSystemCore::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) { m_Evaluater->addMultipleEntityPotentials(std::move(potentials)); }
void ClassicalSystemCore::addNonPotentialForce( ClassicNonPotentialForce NonPotForce ) { m_Evaluater->addNonPotentialForce(std::move(NonPotForce)); }
void ClassicalSystemCore::addMultipleNonPotentialForce( std::vector<ClassicNonPotentialForce> NonPotForce ) { m_Evaluater->addMulitpleNonPotentialForce(std::move(NonPotForce)); }

void ClassicalSystemCore::Step()
{
//Benchmarking
    auto t1 = std::chrono::steady_clock::now();
    m_Integrator->step(m_CurrentState, m_NextState, m_Evaluater, m_Time, m_DeltaTime);
    auto t2 = std::chrono::steady_clock::now();
    auto d1 = t2-t1;
    m_CoreStepTime += std::chrono::duration<double>(d1).count();

    auto t3 = std::chrono::steady_clock::now();
    m_DynamicEntImpact->ApplyImpacts(m_NextState, m_NextState.getDynamicEntitys());
    m_StaticEntImpact->ApplyImpacts(m_NextState, m_NextState.getStaticEntitys());
    auto t4 = std::chrono::steady_clock::now();
    auto d2 = t4-t3;
    m_CoreCollisionTime += std::chrono::duration<double>(d1).count();

    auto t5 = std::chrono::steady_clock::now();
    advanceTimeIncrement();
    
    m_Evaluater->UpdateAccelerations(m_NextState, m_Time);
    m_Evaluater->UpdateKineticEnergys(m_NextState);
    m_Evaluater->UpdatePotentialEnergys(m_NextState, m_Time);

    std::swap(m_NextState, m_CurrentState);
    auto t6 = std::chrono::steady_clock::now();
    auto d3 = t4-t3;
    m_CoreRestTim += std::chrono::duration<double>(d1).count();
}

void ClassicalSystemCore::UpdateEntityPropertys()
{
    m_Evaluater->UpdateAccelerations(m_CurrentState, m_Time);
    m_Evaluater->UpdateKineticEnergys(m_CurrentState);
    m_Evaluater->UpdatePotentialEnergys(m_CurrentState, m_Time);
}

void ClassicalSystemCore::advanceTimeIncrement() { m_Time += m_DeltaTime; }

}
