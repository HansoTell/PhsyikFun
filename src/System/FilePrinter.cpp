#include "Printer.h"
#include <cstddef>
#include <string>

namespace Physik 
{
FilePrinter::FilePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath ) : m_FilePath(std::move(filepath)), m_SystemCore(SystemCore) 
{
    m_File.open(m_FilePath, std::ios::app);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";
}
FilePrinter::FilePrinter( const FilePrinter& other ) {}

FilePrinter::FilePrinter( FilePrinter&& other ) : m_FilePath(std::move(other.m_FilePath)), m_SystemCore(std::move(other.m_SystemCore))
{
    m_File.open(m_FilePath, std::ios::app);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";
}

void FilePrinter::printEntityPositions() const  
{
    const auto& entitys =  m_SystemCore->getEntitys();
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        auto pos = entitys[i].getPosition();
        m_File << i+1 << ", ";
        for( size_t j = 0; j < pos.size(); j++ )
        {
            m_File << std::to_string(pos[j]);
            if( j != pos.size()-1 )
                m_File << ", ";
        }
        m_File << "\n";
    }
}
void FilePrinter::printEnergy() const 
{
    //TODO:

}
void FilePrinter::printForce() const 
{
    //TODO:

}
    
}
