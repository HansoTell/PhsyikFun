#include "Printer.h"
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace Physik 
{
static constexpr uint64_t buff_size = 5'000'000;
CSVFileWriter::CSVFileWriter( std::string FilePath ) : m_FilePath(std::move(FilePath)), m_Options(PrintOptions::eAll) 
{
    m_Buffer.reserve(buff_size);

    m_File.open(m_FilePath, std::ios::trunc);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";

    printEntityStateHeader();
}

CSVFileWriter::CSVFileWriter( std::string FilePath, PrintOptions options ) : m_FilePath(std::move(FilePath)), m_Options(options)
{
    m_Buffer.reserve(buff_size);

    m_File.open(m_FilePath, std::ios::trunc);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";

    printEntityStateHeader();
}

void CSVFileWriter::WriteState( const ClassicEntity& State, double Time ) const
{
    if( m_Buffer.size() + 300 > buff_size )
        flush();
    
    PrintNumber(State.getID());
    PrintSeperator();
    PrintNumber(Time);
    if( has(m_Options, PrintOptions::ePosition ))
    {
        PrintSeperator();
        PrintVector(State.getPosition());
    }
    if( has(m_Options, PrintOptions::eVelocity ))
    {
        PrintSeperator();
        PrintVector(State.getVelocity());
    }
    if( has(m_Options, PrintOptions::eAcceleration ))
    {
        PrintSeperator();
        PrintVector(State.getAcceleration());
    }
    if( has(m_Options, PrintOptions::eForce ))
    {
        PrintSeperator();
        PrintVector(State.getAcceleration() * State.getMass());
    }
    if( has(m_Options, PrintOptions::eKinEnergy ))
    {
        PrintSeperator();
        PrintNumber(State.getKineticEnergy());
    }
    if( has(m_Options, PrintOptions::ePotEnergy ))
    {
        PrintSeperator();
        PrintNumber(State.getPotentialEnergy());
    }

    PrintLineEnd();
}

void CSVFileWriter::printEntityStateHeader() const 
{
    m_Buffer.append("index,Time");
    if( has(m_Options, PrintOptions::ePosition ))
    {
        PrintSeperator();
        m_Buffer.append("pos_x,pos_y,pos_z");
    }
    if( has(m_Options, PrintOptions::eVelocity ))
    {
        PrintSeperator();
        m_Buffer.append("veloc_x,veloc_y,veloc_z");
    }
    if( has(m_Options, PrintOptions::eAcceleration ))
    {
        PrintSeperator();
        m_Buffer.append("acc_x,acc_y,acc_z");
    }
    if( has(m_Options, PrintOptions::eForce ))
    {
        PrintSeperator();
        m_Buffer.append("F_x,F_y,F_z");
    }
    if( has(m_Options, PrintOptions::eKinEnergy ))
    {
        PrintSeperator();
        m_Buffer.append("Ekin");
    }
    if( has(m_Options, PrintOptions::ePotEnergy ))
    {
        PrintSeperator();
        m_Buffer.append("EPot");
    }

    PrintLineEnd();
}

void CSVFileWriter::PrintVector( const Vec3D& vector ) const
{
    for( size_t i = 0; i < vector.size(); i++ )
    {
        PrintNumber( vector[i] );
        if( i != vector.size()-1 )
           PrintSeperator(); 
    }
}

void CSVFileWriter::PrintSeperator() const { m_Buffer += ","; }
void CSVFileWriter::PrintLineEnd() const { m_Buffer += "\n"; }

void CSVFileWriter::flush() const
{
    std::cout << "Flushed with size: " << m_Buffer.size() << "\n";
    m_File << m_Buffer;
    m_Buffer.clear();
    m_File.flush();
}
}
