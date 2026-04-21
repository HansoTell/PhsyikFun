#include "Entity.h"
#include <System.h>
#include <memory>


namespace Physik 
{
ClassicalSystem::ClassicalSystem() 
{
    m_Printer = std::make_unique<ConsolePrinter>();
}

ClassicalSystem::ClassicalSystem( std::unique_ptr<IPrinter> printer) : m_Printer(std::move(printer)) 
{

}

ClassicalSystem::ClassicalSystem( const ClassicalSystem& other )
{

}

void ClassicalSystem::Run() 
{
    while( m_running )
    {

    }
}

void ClassicalSystem::addPotential( std::unique_ptr<ClassicPotential> potential )
{
    m_Potentials.push_back(std::move(potential)); 
}

void ClassicalSystem::addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials )
{
    m_Potentials.insert(m_Potentials.cend(), potentials.begin(), potentials.end());
}

void ClassicalSystem::addEntity( ClassicEntity entity )
{
    m_Entitys.push_back(std::move(entity));
}

void ClassicalSystem::addMulipleEntitys( std::vector<ClassicEntity> entitys )
{
    m_Entitys.insert(m_Entitys.cend(), entitys.begin(), entitys.end());
}


}
