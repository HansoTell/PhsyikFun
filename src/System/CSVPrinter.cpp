#include "Printer.h"
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

namespace Physik 
{
CSVPrinter::CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath ) 
    :  m_SystemCore(SystemCore), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(filepath))) {}

CSVPrinter::CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath, PrintOptions options )
    : m_SystemCore(SystemCore), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(filepath), options)) {}

void CSVPrinter::Print() const 
{
    const auto& AllEntitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < AllEntitys.size(); i++ )
    {
        const auto& entity = AllEntitys[i];
        m_FileWriter->WriteState(entity.getEntityState(), entity.getID(), m_SystemCore->getTime());
    }
}
}


