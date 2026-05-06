#include "Entity.h"
#include "Interactions.h"
#include "SystemCore.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

namespace Physik 
{

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> dglMethod ) 
    : m_DeltaTime(DEFAULT_DELTA_TIME), m_PropertyCalcer(std::move(dglMethod)), m_Time(0.0) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> dglMethod, double deltaTime ) 
    : m_DeltaTime( deltaTime ), m_PropertyCalcer(std::move(dglMethod)), m_Time(0.0) 
{
    UpdateEntityPropertys();
}

ClassicalSystemCore::ClassicalSystemCore( const ClassicalSystemCore& other ) 
    : m_DeltaTime(other.m_DeltaTime), m_Time(other.m_Time), Energy(other.Energy),
    m_EntityPotentials(other.m_EntityPotentials), m_Entitys(other.m_Entitys), m_ExtPotentials(other.m_ExtPotentials), m_PropertyCalcer(other.m_PropertyCalcer->clone()) {}

ClassicalSystemCore::ClassicalSystemCore( ClassicalSystemCore&& other ) 
    : m_DeltaTime(other.m_DeltaTime), m_Time(other.m_Time), Energy(other.Energy), 
    m_PropertyCalcer(std::move(other.m_PropertyCalcer)), 
    m_EntityPotentials(std::move(other.m_EntityPotentials)), m_Entitys(std::move(other.m_Entitys)), m_ExtPotentials(std::move(other.m_ExtPotentials)) {}

void ClassicalSystemCore::Clear() 
{
    m_Entitys.clear();
    m_ExtPotentials.clear();
    m_Time = 0.0;
    m_DeltaTime = DEFAULT_DELTA_TIME;
    Energy = 0.0; 
}

void ClassicalSystemCore::addEntity( ClassicEntity entity ) { m_Entitys.push_back(std::move(entity)); }
void ClassicalSystemCore::addMulipleEntitys( std::vector<ClassicEntity> entitys ) { for( int i = 0; i < entitys.size(); i++ ) m_Entitys.push_back(std::move(entitys[i])); }
void ClassicalSystemCore::addExternPotential( ClassicField extPotential ) { m_ExtPotentials.push_back(ClassicField(std::move(extPotential))); }
void ClassicalSystemCore::addMulitpleExternPotentials( std::vector<ClassicField> potentials ) { for( int i = 0; i < potentials.size(); i++ ) m_ExtPotentials.push_back(std::move(potentials[i])); }
void ClassicalSystemCore::addEntityPotential( ClassicInteraction potential ) { m_EntityPotentials.push_back(std::move(potential)); }
void ClassicalSystemCore::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) { for( size_t i = 0; i < potentials.size(); i++) m_EntityPotentials.push_back(std::move(potentials[i])); }


void ClassicalSystemCore::MakeTimeStep()
{
    advanceTimeIncrement();

    const auto NewEntityStates = ClacNewEntityStates();

    ApplyNewEntityStates(NewEntityStates);

    UpdateEnergy();
}

void ClassicalSystemCore::advanceTimeIncrement() { m_Time += m_DeltaTime; }

void ClassicalSystemCore::ApplyAllForces( std::vector<ClassicEntityState>& outPropertys ) const
{
    for( auto& interaction : m_EntityPotentials )
        interaction.applyForceOnAllEntitys( m_Entitys, outPropertys, m_Time);

    for( auto& field : m_ExtPotentials )
        field.applyForceOnAllEntitys(m_Entitys, outPropertys, m_Time);
}

void ClassicalSystemCore::ApplyAllEnergy( std::vector<ClassicEntityState>& outPropertys ) const
{
    for( auto& interaction : m_EntityPotentials )
        interaction.ApplyPotEnergyOnAllEntitys(m_Entitys, outPropertys, m_Time);
    for( auto& field : m_ExtPotentials )
        field.ApplyPotEnergyOnAllEntitys(m_Entitys, outPropertys, m_Time);
    
    m_PropertyCalcer->ApplyAllKineticEnergy(m_Entitys, outPropertys);
}

//Geführlich Weil ptr wird ja mit kopiert nicht der inhalt also ziegt n9ch auf gleiche speuicherstelle
static std::vector<ClassicEntityState> CreateEntityStateList( const std::vector<ClassicEntity>& entitys )
{
    std::vector<ClassicEntityState> AllEntityChanges;
    AllEntityChanges.reserve(entitys.size());

    for(const auto& entity : entitys )
        AllEntityChanges.push_back(entity.getEntityStateCopy());

    return AllEntityChanges;
}

std::vector<ClassicEntityState> ClassicalSystemCore::ClacNewEntityStates() const 
{
    std::vector<ClassicEntityState> AllEntityChanges = CreateEntityStateList(m_Entitys);

    ApplyAllForces(AllEntityChanges);

    m_PropertyCalcer->ApplyAllAcceleration(m_Entitys, AllEntityChanges);
    m_PropertyCalcer->ApplyAllNewtonIntegration(m_Entitys, AllEntityChanges, m_DeltaTime);
    
    ApplyAllEnergy(AllEntityChanges);

    return AllEntityChanges;
}

void ClassicalSystemCore::ApplyNewEntityStates( const std::vector<ClassicEntityState>& Propertys )
{
    assert(Propertys.size() == m_Entitys.size());
    for( size_t i = 0; i < Propertys.size(); i++ )
        m_Entitys[i].setEntityState(Propertys[i]);
}

void ClassicalSystemCore::UpdateEnergy()
{
    double Energy_Tot = 0.0;
    for( const auto& entity : m_Entitys )
        Energy_Tot += entity.getEnergy();
    
    Energy = Energy_Tot;
}

void ClassicalSystemCore::UpdateEntityPropertys() 
{
    std::vector<ClassicEntityState> AllEntityChanges = CreateEntityStateList(m_Entitys);

    ApplyAllForces(AllEntityChanges);
    m_PropertyCalcer->ApplyAllAcceleration(m_Entitys, AllEntityChanges);
    ApplyAllEnergy(AllEntityChanges);

    ApplyNewEntityStates(AllEntityChanges);

    UpdateEnergy();
}
}
