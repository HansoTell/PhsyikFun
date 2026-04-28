#include "Entity.h"
#include "System.h"
#include <memory>
#include <vector>

namespace Physik 
{

ClassicalSystemCore::ClassicalSystemCore() : m_DelatTime(DEFAULT_DELTA_TIME) {}

ClassicalSystemCore::ClassicalSystemCore( double deltaTime ) : m_DelatTime( deltaTime ) {}

ClassicalSystemCore::ClassicalSystemCore( const ClassicalSystemCore& other ) {}

ClassicalSystemCore::ClassicalSystemCore( ClassicalSystemCore&& other ) {}

void ClassicalSystemCore::Clear() 
{
    m_Entitys.clear();
    m_Potentials.clear();
    m_Time = 0.0;
    m_DelatTime = DEFAULT_DELTA_TIME;
    Energy = 0.0; 
}

void ClassicalSystemCore::addEntity( ClassicEntity entity ) { m_Entitys.push_back(std::move(entity)); }
void ClassicalSystemCore::addPotential( std::unique_ptr<ClassicPotential> potential ) { m_Potentials.push_back(std::move(potential)); }
void ClassicalSystemCore::addMulipleEntitys( std::vector<ClassicEntity> entitys ) { for( int i = 0; i < entitys.size(); i++ ) m_Entitys.push_back(std::move(entitys[i])); }
void ClassicalSystemCore::addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials ) { for( int i = 0; i < potentials.size(); i++ ) m_Potentials.push_back(std::move(potentials[i])); }
}
