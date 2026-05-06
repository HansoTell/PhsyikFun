#include "Entity.h"
#include "Interactions.h"
#include "SystemCore.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

namespace Physik 
{

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> dglMethod ) : m_DeltaTime(DEFAULT_DELTA_TIME), m_PropertyCalcer(std::move(dglMethod)) {}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> dglMethod, double deltaTime ) : m_DeltaTime( deltaTime ), m_PropertyCalcer(std::move(dglMethod)) {}

//TODO:
ClassicalSystemCore::ClassicalSystemCore( const ClassicalSystemCore& other ) {}

ClassicalSystemCore::ClassicalSystemCore( ClassicalSystemCore&& other ) {}

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

void ClassicalSystemCore::advanceTimeIncrement() { m_Time += m_DeltaTime; }

void ClassicalSystemCore::moveEntitys()
{
    const auto EntityChanges = ClacEffektOfPotentials();

    ApplyMovementOnEntitys(EntityChanges);
}

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

std::vector<ClassicEntityState> ClassicalSystemCore::ClacEffektOfPotentials() const 
{
    //TODO: Es fehlen die nicht modifizierten attribute wie masse u.s.w, dass die kopiert werden
    std::vector<ClassicEntityState> AllEntityChanges(m_Entitys.size());

    ApplyAllForces(AllEntityChanges);

    m_PropertyCalcer->ApplyAllAcceleration(m_Entitys, AllEntityChanges);
    m_PropertyCalcer->ApplyAllNewtonIntegration(m_Entitys, AllEntityChanges);
    
    ApplyAllEnergy(AllEntityChanges);

    return AllEntityChanges;
}

void ClassicalSystemCore::ApplyMovementOnEntitys( const std::vector<ClassicEntityState>& Propertys )
{
    assert(Propertys.size() == m_Entitys.size());
    for( size_t i = 0; i < Propertys.size(); i++ )
    {
        //TODO: Alle werte setzten --> fehlende set Methoden adden oder alle entf3rnen und kopieren des property states ok machen

    }
}

void ClassicalSystemCore::UpdateEnergy()
{
    double Energy_Tot = 0.0;
    for( const auto& entity : m_Entitys )
        Energy_Tot += entity.getEnergy();
    
    Energy = Energy_Tot;
}
}
