#include "Interactions.h"
#include "SystemCore.h"
#include "Vector.h"

#include <cstddef>
#include <vector>


#include <iostream>

namespace Physik 
{

ClassicalSystemCore::ClassicalSystemCore() : m_DeltaTime(DEFAULT_DELTA_TIME) {}

ClassicalSystemCore::ClassicalSystemCore( double deltaTime ) : m_DeltaTime( deltaTime ) {}

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

std::vector<ClassicEntityPropertys> ClassicalSystemCore::ClacEffektOfPotentials() const 
{
    std::vector<ClassicEntityPropertys> AllEntityChanges(m_Entitys.size());
    for( int i = 0; i < m_Entitys.size(); i++ )
    {
        const auto& entitys = m_Entitys[i]; 

        CalcEffectOnEntity( entitys, i, AllEntityChanges );
        std::cout << AllEntityChanges[i].Force << "\n";
    }

    return AllEntityChanges;
}

void ClassicalSystemCore::CalcEffectOnEntity( const ClassicEntity& entitys, size_t idx, std::vector<ClassicEntityPropertys>& outPropertys ) const
{

    auto& Property_idx = outPropertys[idx];

    Property_idx.EntityIndex = idx;

    for( size_t j = idx+1; j < m_Entitys.size(); j++ )
    {
        Vec3D force = CalcForceOfEntityPotentials(m_EntityPotentials, entitys, m_Entitys[j]);

        Property_idx.Force += force;
        outPropertys[j].Force += force * -1;
    }
    Property_idx.Force += CalcForceOfExtPotentials(m_ExtPotentials, entitys);

    Property_idx.Acceleration = Property_idx.Force * (1/entitys.getMass());
    Property_idx.deltaVelocity = Property_idx.Acceleration * m_DeltaTime;
    Property_idx.deltaPosition = (entitys.getVelocity() + Property_idx.deltaVelocity) * m_DeltaTime;

}

Vec3D ClassicalSystemCore::CalcForceOfEntityPotentials( const std::vector<ClassicInteraction>& potentials, const ClassicEntity& ent1, const ClassicEntity& ent2) const
{
    Vec3D Force_erg;

    for( const auto& potential : potentials )
        Force_erg += potential.getForce( ent1.getEntityState(), ent2.getEntityState(), m_Time);
    
    return Force_erg;
}

Vec3D ClassicalSystemCore::CalcForceOfExtPotentials( const std::vector<ClassicField>& potentials, const ClassicEntity& entity ) const
{
    Vec3D Force_ges;
    for( const auto& potential : potentials ) 
        Force_ges += potential.getForce(entity.getEntityState(), m_Time);
    
    return Force_ges;
}

void ClassicalSystemCore::ApplyMovementOnEntitys( const std::vector<ClassicEntityPropertys>& Propertys )
{
    for( const auto& prop : Propertys )
    {
        auto& entity = m_Entitys[prop.EntityIndex];
        entity.setVelocity(entity.getVelocity() + prop.deltaVelocity);
        entity.setPosition(entity.getPosition() + prop.deltaPosition);
    }
}
}
