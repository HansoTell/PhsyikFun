#include "Printer.h"
#include <cstddef>
#include <iostream>


namespace Physik 
{

ConsolePrinter::ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore ) : m_SystemCore(SystemCore){}

void ConsolePrinter::printPosition() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( int i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << entity.getID() << " At Position: " << entity.getPosition() << "\n";
    }

} 
void ConsolePrinter::printVelocity() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << entity.getID() << " Velocity: " << entity.getVelocity() << "\n";
    }
}

void ConsolePrinter::printAcceleration() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << entity.getID() << " Acceleration: " << entity.getAcceleration() << "\n";
    }
}

void ConsolePrinter::printForce() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        std::cout << "Entity " << entitys[i].getID() << " With Force: " << entitys[i].getForce() << " N" << "\n";
    }
}

void ConsolePrinter::printEnergy() const 
{
    std::cout << "Energy: " << m_SystemCore->getEnergy() << "J\n";
}

void ConsolePrinter::printAll() const 
{
    printPosition();
    printVelocity();
    printAcceleration();
    printForce();
    printEnergy();
}
}


