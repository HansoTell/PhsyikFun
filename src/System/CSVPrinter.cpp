#include "Entity.h"
#include "Printer.h"
#include <cstddef>
#include <cstdio>
#include <string>
#include <iostream>
#include <ios>


//Problem wie machen wir das mit den methodne die nicht interfaced sind? Also was machen wir damit? einfach leer uind sagen ja juckt?
namespace Physik 
{
CSVPrinter::CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath ) 
    : m_FilePath(std::move(filepath)), m_SystemCore(SystemCore), counter(0) 
{
    m_Buffer.reserve(10000);

    m_File.open(m_FilePath, std::ios::app);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";

    printEntityStateHeader();

}
CSVPrinter::CSVPrinter( const CSVPrinter& other ) {}

CSVPrinter::CSVPrinter( CSVPrinter&& other ) : m_FilePath(std::move(other.m_FilePath)), m_SystemCore(std::move(other.m_SystemCore))
{
    m_File.open(m_FilePath, std::ios::app);
    if(!m_File.is_open())
        std::cerr << "Filed to open File" << "\n";
}

void CSVPrinter::printEntityPositions() const  
{
    //TODO:
}
void CSVPrinter::printEnergy() const 
{
    //TODO:

}
void CSVPrinter::printForce() const 
{
    //TODO:

}


void CSVPrinter::printEntityStateHeader() const
{
    m_Buffer.append("index, pos_x, pos_y, pox_z, " 
         "veloc_x, veloc_y, veloc_z, " 
         "acc_x, acc_y, acc_z, " 
         "F_x, F_y, F_z, " 
         "Mass, EKin, EPot\n");
}

void CSVPrinter::PrintVector( const Vec3D& vector ) const
{
    for( size_t i = 0; i < vector.size(); i++ )
    {
        m_Buffer += vector[i];
        if( i != vector.size()-1 )
            m_Buffer += ", ";
    }
}

void CSVPrinter::printAll() const 
{
    const auto& AllEntitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < AllEntitys.size(); i++ )
    {
        const auto& State = AllEntitys[i].getEntityState();
        m_Buffer += std::to_string(i+1);
        PrintSeperator();
        PrintVector(*State.m_Position);
        PrintSeperator();
        PrintVector(State.m_Velocity);
        PrintSeperator();
        PrintVector(State.m_Acceleration);
        PrintSeperator();
        PrintVector(State.m_Force);
        PrintSeperator();

        (((((m_Buffer += State.m_Mass) += ", ") += State.KineticEnergy) += ", ") += State.PotentialEnergy);
        PrintLineEnd();
    }

    if( counter >= 100 )
        flush();

    counter++;
}

void CSVPrinter::PrintSeperator() const { m_Buffer += ", "; }
void CSVPrinter::PrintLineEnd() const { m_Buffer += "\n"; }

void CSVPrinter::flush() const 
{
    m_File << m_Buffer;
    m_Buffer.clear();
    m_File.flush();
}
    
}

