#include "Printer.h"
#include <cstddef>
#include <iostream>


namespace Physik 
{

ConsolePrinter::ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore ) : m_SystemCore(SystemCore), m_Options(PrintOptions::eAll){}
ConsolePrinter::ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, PrintOptions options ) : m_SystemCore(SystemCore), m_Options(options) {}

void ConsolePrinter::Print() const 
{
    if( has(m_Options, PrintOptions::ePosition) )
        printPosition();
    if( has(m_Options, PrintOptions::eVelocity) )
        printVelocity();
    if( has(m_Options, PrintOptions::eAcceleration) )
        printAcceleration();
    if( has(m_Options, PrintOptions::eForce) )
        printForce();
    if( has(m_Options, PrintOptions::eKinEnergy) )
        printKineticEnergy();
    if( has(m_Options, PrintOptions::ePotEnergy) )
        printKineticEnergy();
}

void ConsolePrinter::printPosition() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( int i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << entity.getID() << " At Position: " << entity.getPosition() << " m" << "\n";
    }

} 
void ConsolePrinter::printVelocity() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << entity.getID() << " Velocity: " << entity.getVelocity() <<  " m/s" << "\n";
    }
}

void ConsolePrinter::printAcceleration() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << entity.getID() << " Acceleration: " << entity.getAcceleration() << " m/s*s" << "\n";
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

void ConsolePrinter::printKineticEnergy() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
        std::cout << "Entity " << entitys[i].getID() << " With Kinetic Energy: " << entitys[i].getEntityState().KineticEnergy << " J" << "\n";
}

void ConsolePrinter::printPotentialEnergy() const
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
        std::cout << "Entity " << entitys[i].getID() << " With Potential Energy: " << entitys[i].getEntityState().PotentialEnergy<< " J" << "\n";

}

}


