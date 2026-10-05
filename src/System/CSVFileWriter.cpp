#include "Entity.h"
#include "Printer.h"
#include "Vector.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

namespace Physik 
{
namespace PrintingHelpers 
{

    template<typename T>
    inline static std::string PrintNumber( T number ) 
    {
        static_assert(std::is_arithmetic_v<T>);

        return std::to_string(number);
    }

    inline static std::string PrintSeperator()
    {
        return ",";
    }
    template<size_t Dim, typename T>
    static std::string PrintVector(const Vector<Dim, T>& vector )
    {
        std::string erg;
        for( size_t i = 0; i < vector.size(); i++ )
        {
            erg += PrintNumber( vector[i] );
            if( i != vector.size()-1 )
               erg += PrintSeperator(); 
        }
        return erg;
    }
    static inline std::string PrintLineEnd(){ return "\n"; }
}

struct OptionEntry 
{
    PrintOptions flag;
    std::string_view label;
    std::string (*format)(const ClassicEntity&);
};

constexpr std::array<OptionEntry, 9> kOptionTable
{{
    { PrintOptions::ePosition, ",pos_x,pos_y,pos_z", [](const ClassicEntity& ent) { return PrintingHelpers::PrintVector(ent.getPosition()); } },
    { PrintOptions::eVelocity, ",veloc_x,veloc_y,veloc_z", [](const ClassicEntity& ent){ return PrintingHelpers::PrintVector(ent.getVelocity()); } },
    { PrintOptions::eAcceleration, ",acc_x,acc_y,acc_z", [](const ClassicEntity& ent) { return PrintingHelpers::PrintVector(ent.getAcceleration()); } },
    { PrintOptions::eRotation, ",angel,rot_x,rot_y,rot_z", [](const ClassicEntity& ent) { return PrintingHelpers::PrintVector(ent.getRotation().getAsVector()); } }, 
    { PrintOptions::eAngularVelocity, ",AngVeloc_x,AngVeloc_y,AngVeloc_z", [](const ClassicEntity& ent) { return PrintingHelpers::PrintVector(ent.getAngularVelocity()); } },
    { PrintOptions::eAngularAcceleration, ",AngAcc_x,AngAcc_y,AngAcc_z", [](const ClassicEntity& ent) { return PrintingHelpers::PrintVector(ent.getAngularAcceleration()); } },
    { PrintOptions::eForce, ",F_x,F_y,F_z", [](const ClassicEntity& ent) { return PrintingHelpers::PrintVector(ent.getMass() * ent.getAcceleration()); } },
    { PrintOptions::eKinEnergy, ",Ekin", [](const ClassicEntity& ent) { return PrintingHelpers::PrintNumber(ent.getKineticEnergy()); } },
    { PrintOptions::ePotEnergy, ",EPot", [](const ClassicEntity& ent) { return PrintingHelpers::PrintNumber(ent.getPotentialEnergy());  } }
}};

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
    
    m_Buffer += PrintingHelpers::PrintNumber(State.getID());
    m_Buffer += PrintingHelpers::PrintSeperator();
    m_Buffer += PrintingHelpers::PrintNumber(Time);

    for( const auto& entry : kOptionTable )
    {
        if( has(m_Options, entry.flag) )
        {
            m_Buffer += PrintingHelpers::PrintSeperator();
            m_Buffer += entry.format(State);
        }
    }

    m_Buffer += PrintingHelpers::PrintLineEnd();
}

void CSVFileWriter::printEntityStateHeader() const 
{
    m_Buffer.append("index,Time");

    for( const auto& entry : kOptionTable )
    {
        if( has(m_Options, entry.flag))
            m_Buffer.append(entry.label);
    }

    m_Buffer += PrintingHelpers::PrintLineEnd();
}

void CSVFileWriter::flush() const
{
    std::cout << "Flushed with size: " << m_Buffer.size() << "\n";
    m_File << m_Buffer;
    m_Buffer.clear();
    m_File.flush();
}
}
