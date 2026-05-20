#include "Printer.h"
#include <cstddef>
#include <cstdint>
#include <iostream>

#define BUFF_SIZE 5000000
namespace Physik 
{
CSVFileWriter::CSVFileWriter( std::string FilePath ) : m_FilePath(std::move(FilePath)) 
{
    m_Buffer.reserve(BUFF_SIZE);

    m_File.open(m_FilePath, std::ios::trunc);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";

    printEntityStateHeader();
}

void CSVFileWriter::WriteState( const ClassicEntityState& State, uint64_t ID ) const
{
    if( m_Buffer.size() + 300 > BUFF_SIZE )
        flush();
    
    PrintNumber(ID);
    PrintSeperator();
    PrintVector(State.m_Position);
    PrintSeperator();
    PrintVector(State.m_Velocity);
    PrintSeperator();
    PrintVector(State.m_Acceleration);
    PrintSeperator();
    PrintVector(State.m_Force);
    PrintSeperator();
    PrintNumber(State.m_Mass);
    PrintSeperator();
    PrintNumber(State.KineticEnergy);
    PrintSeperator();
    PrintNumber(State.PotentialEnergy);
    PrintLineEnd();
}

void CSVFileWriter::printEntityStateHeader() const 
{
    m_Buffer.append("index, pos_x, pos_y, pox_z, " 
         "veloc_x, veloc_y, veloc_z, " 
         "acc_x, acc_y, acc_z, " 
         "F_x, F_y, F_z, " 
         "Mass, EKin, EPot\n");
}

void CSVFileWriter::PrintVector( const Vec3D& vector ) const
{
    for( size_t i = 0; i < vector.size(); i++ )
    {
        PrintNumber( vector[i] );
        if( i != vector.size()-1 )
            m_Buffer += ", ";
    }
}

void CSVFileWriter::PrintSeperator() const { m_Buffer += ", "; }
void CSVFileWriter::PrintLineEnd() const { m_Buffer += "\n"; }

void CSVFileWriter::flush() const
{
    std::cout << "Flushed with size: " << m_Buffer.size() << "\n";
    m_File << m_Buffer;
    m_Buffer.clear();
    m_File.flush();
}
}
