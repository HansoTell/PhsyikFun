#include "SystemCore.h"


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
void ClassicalSystemCore::addPotential( std::unique_ptr<ClassicPotential> potential ) { m_ExtPotentials.push_back(std::move(potential)); }
void ClassicalSystemCore::addMulipleEntitys( std::vector<ClassicEntity> entitys ) { for( int i = 0; i < entitys.size(); i++ ) m_Entitys.push_back(std::move(entitys[i])); }
void ClassicalSystemCore::addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials ) { for( int i = 0; i < potentials.size(); i++ ) m_ExtPotentials.push_back(std::move(potentials[i])); }


void ClassicalSystemCore::advanceTimeIncrement() { m_Time += m_DeltaTime; }

void ClassicalSystemCore::moveEntitys()
{
    const auto EntityChanges = ClacEffektOfPotentials();
    std::cout << "Fordce: " << EntityChanges[0].Force << " Acc: " << EntityChanges[0].Acceleration << " veloc " << EntityChanges[0].deltaVelocity << " pos " << EntityChanges[0].deltaPosition << "\n";

    ApplyMovementOnEntitys(EntityChanges);
}

std::vector<ClassicEntityPropertys> ClassicalSystemCore::ClacEffektOfPotentials() const 
{
    std::vector<ClassicEntityPropertys> AllEntityChanges;
    AllEntityChanges.reserve(m_Entitys.size());
    for( int i = 0; i < m_Entitys.size(); i++ )
    {
        const auto& entitys = m_Entitys[i]; 

        ClassicEntityPropertys Propertys = CalcEffectOnEntity( entitys, i );
        AllEntityChanges.push_back(Propertys);
    }

    return AllEntityChanges;
}

ClassicEntityPropertys ClassicalSystemCore::CalcEffectOnEntity( const ClassicEntity& entitys, size_t idx ) const
{
    ClassicEntityPropertys Propertys;
    Propertys.EntityIndex = idx;

    for( int j = 0; j < m_Entitys.size(); j++ )
    {
        if( Propertys.EntityIndex == j)
            continue;

        const auto& IntrinsicPotentials = m_Entitys[j].getIntrinsicPotentials();
        CalcForceOfPotentialsOnEntity(IntrinsicPotentials, entitys, Propertys);
    }
    CalcForceOfPotentialsOnEntity(m_ExtPotentials, entitys, Propertys);


    Propertys.Acceleration = Propertys.Force * (1/entitys.getMass());
    Propertys.deltaVelocity = Propertys.Acceleration * m_DeltaTime;
    Propertys.deltaPosition = (entitys.getVelocity() + Propertys.deltaVelocity) * m_DeltaTime;

    return Propertys;
}

void ClassicalSystemCore::CalcForceOfPotentialsOnEntity( const std::vector<std::unique_ptr<IPotential<3, double>>>& potentials, const ClassicEntity& entity, ClassicEntityPropertys& outPropertys ) const
{
    for( const auto& potential : potentials ) 
    {
        outPropertys.Force += potential->getForce(entity.getPosition(), entity.getMass(), m_Time);
    }
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
