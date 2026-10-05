#include "Printer.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <unordered_map>

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

struct OptionEntry 
{
    PrintOptions flag;
    std::string_view label;
};

constexpr std::array<OptionEntry, 9> kOptionTable
{{
    { PrintOptions::ePosition, ",pos_x,pos_y,pos_z" },
    { PrintOptions::eVelocity, ",veloc_x,veloc_y,veloc_z" },
    { PrintOptions::eAcceleration, ",acc_x,acc_y,acc_z" },
    { PrintOptions::eRotation, "Keine Ahnung was man hier printed" }, //TODO:
    { PrintOptions::eAngularVelocity, ",AngVeloc_x,AngVeloc_y,AngVeloc_z" },
    { PrintOptions::eAngularAcceleration, ",AngAcc_x,AngAcc_y,AngAcc_z" },
    { PrintOptions::eForce, ",F_x,F_y,F_z" },
    { PrintOptions::eKinEnergy, ",Ekin" },
    { PrintOptions::ePotEnergy, ",EPot" }
}};

void CSVFileWriter::printEntityStateHeader() const 
{
    m_Buffer.append("index,Time");

    for( const auto&[flag, label] : kOptionTable )
    {
        if( has(m_Options, flag))
            m_Buffer.append(label);
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
