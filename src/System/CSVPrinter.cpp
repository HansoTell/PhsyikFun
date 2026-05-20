#include "Printer.h"
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

namespace Physik 
{
CSVPrinter::CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath ) 
    :  m_SystemCore(SystemCore), counter(0), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(filepath))) {}

//TODO: Möglichkeiten: 
//2. Also dass Man enum hat mit optionen -> 
//3. --> 2. aber Man muss beim start das reingeben oder in konstruktor anbieten der einstellen lässt, daraus dann die optionen ergeben
void CSVPrinter::printPosition() const{}
void CSVPrinter::printVelocity() const {}
void CSVPrinter::printAcceleration() const {}
void CSVPrinter::printForce() const {}
void CSVPrinter::printEnergy() const {}

void CSVPrinter::printAll() const 
{
    const auto& AllEntitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < AllEntitys.size(); i++ )
    {
        const auto& entity = AllEntitys[i];
        m_FileWriter->WriteState(entity.getEntityState(), entity.getID());
    }
}
}


