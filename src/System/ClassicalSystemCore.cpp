#include "Interactions.h"
#include "SystemCore.h"
#include "Vector.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace Physik 
{

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod ) : m_DeltaTime(DEFAULT_DELTA_TIME), m_DGLMethod(std::move(dglMethod)) {}

ClassicalSystemCore::ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod, double deltaTime ) : m_DeltaTime( deltaTime ), m_DGLMethod(std::move(dglMethod)) {}

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

std::vector<ClassicEntityPropertys> ClassicalSystemCore::ClacEffektOfPotentials() const 
{
    std::vector<ClassicEntityPropertys> AllEntityChanges(m_Entitys.size());
    for( int i = 0; i < m_Entitys.size(); i++ )
    {
        const auto& entitys = m_Entitys[i]; 

        CalcEffectOnEntity( entitys, i, AllEntityChanges );
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

        double energy = CalcPotEnergyOfEntityPotentials(m_EntityPotentials, entitys, m_Entitys[j]);
        Property_idx.PotEnergy += energy;
        outPropertys[j].PotEnergy += energy * -1;
    }
    Property_idx.Force += CalcForceOfExtPotentials(m_ExtPotentials, entitys);
    Property_idx.Acceleration = Property_idx.Force * (1/entitys.getMass());
    Property_idx.PotEnergy += CalcPotEnergyOfExtPotentials(m_ExtPotentials, entitys);

    Property_idx.Velocity = m_DGLMethod->CalcNewVelocity(entitys.getVelocity(), Property_idx.Acceleration, m_DeltaTime);
    Property_idx.Position = m_DGLMethod->CalcNewPosition(entitys.getPosition(), Property_idx.Velocity, m_DeltaTime);

    Property_idx.KinEnergy = 0.5 * entitys.getMass() * Property_idx.Velocity.EukNorm() * Property_idx.Velocity.EukNorm();
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
        entity.setVelocity(prop.Velocity);
        entity.setPosition(prop.Position);
        entity.setKineticEnergy(prop.KinEnergy);
        entity.setPotentialEnergy(prop.PotEnergy);
    }
}

void ClassicalSystemCore::UpdateEnergy()
{
    double Energy_Tot = 0.0;
    for( const auto& entity : m_Entitys )
        Energy_Tot += entity.getEnergy();
    
    Energy = Energy_Tot;
}

double ClassicalSystemCore::CalcPotEnergyOfEntityPotentials ( const std::vector<ClassicInteraction>& potentials, const ClassicEntity& ent1, const ClassicEntity& ent2 ) const
{
    double Energy_ges;

    for( const auto& potential : potentials )
        Energy_ges += potential.getPotentialEnergy( ent1.getEntityState(), ent2.getEntityState(), m_Time);
    
    return Energy_ges;
}
double ClassicalSystemCore::CalcPotEnergyOfExtPotentials( const std::vector<ClassicField>& potentials, const ClassicEntity& entity ) const
{
    double Energy_ges;
    for( const auto& potential : potentials ) 
        Energy_ges += potential.getPotentialEnergy(entity.getEntityState(), m_Time);
    
    return Energy_ges;
}

}
