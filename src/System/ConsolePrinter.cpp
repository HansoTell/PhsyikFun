#include "Printer.h"
#include <cstddef>
#include <iostream>


namespace Physik 
{

ConsolePrinter::ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore ) : m_SystemCore(SystemCore){}
//mal gucken
ConsolePrinter::ConsolePrinter( const ConsolePrinter& other ) {}
ConsolePrinter::ConsolePrinter( ConsolePrinter&& other ) {}

void ConsolePrinter::printEntityPositions() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( int i = 0; i < entitys.size(); i++ )
    {
        const auto& entity = entitys[i];

        std::cout << "Entity " << i+1 << " At Position: " << entity.getPosition() << "\n";
    }

} 
void ConsolePrinter::printForce() const 
{
    const auto& entitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        std::cout << "Entity " << i+1 << " With Force: " << entitys[i].getForce() << " N" << "\n";
    }

}

void ConsolePrinter::printEnergy() const 
{
    std::cout << "Energy: " << m_SystemCore->getEnergy() << "J\n";
}

//TODO richtig machen uznd missing methoden adden das man wirklich entschiedne kannw as man printet
void ConsolePrinter::printAll() const 
{
    printEntityPositions();
    printForce();
    printEnergy();
}
}


