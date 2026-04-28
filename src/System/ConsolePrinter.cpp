#include "Printer.h"
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
}
