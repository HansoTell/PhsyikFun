#include "Entity.h"
#include "Printer.h"
#include <cstddef>
#include <cstdio>
#include <mutex>
#include <string>
#include <iostream>
#include <ios>


//Problem wie machen wir das mit den methodne die nicht interfaced sind? Also was machen wir damit? einfach leer uind sagen ja juckt?
#define BUFF_SIZE 25000
namespace Physik 
{
CSVPrinter::CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath ) 
    : m_FilePath(std::move(filepath)), m_SystemCore(SystemCore), counter(0) 
{
    m_Buffer.reserve(BUFF_SIZE);

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
        AppendNumberToBuffer( vector[i] );
        if( i != vector.size()-1 )
            m_Buffer += ", ";
    }
}

void CSVPrinter::printAll() const 
{
    if( m_Buffer.size() > BUFF_SIZE )
        m_Buffer += "uberschritten\n";
    const auto& AllEntitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < AllEntitys.size(); i++ )
    {
        const auto& State = AllEntitys[i].getEntityState();
        AppendNumberToBuffer(i+1);
        PrintSeperator();
        PrintVector(State.m_Position);
        PrintSeperator();
        PrintVector(State.m_Velocity);
        PrintSeperator();
        PrintVector(State.m_Acceleration);
        PrintSeperator();
        PrintVector(State.m_Force);
        PrintSeperator();
        AppendNumberToBuffer(State.m_Mass);
        PrintSeperator();
        AppendNumberToBuffer(State.KineticEnergy);
        PrintSeperator();
        AppendNumberToBuffer(State.PotentialEnergy);
        PrintLineEnd();
    }

    if( counter >= 100 )
    {
        flush();
        counter = 0;
    }

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

AsyncCSVPrinter::AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath ) 
    : m_Printer(CSVPrinter(SystemCore, std::move(FilePath))), m_Queue(), m_SystemCore(SystemCore), m_running(true)
{
    m_Thread = std::thread([this](){ this->Run(); });
}

//AsyncCSVPrinter::AsyncCSVPrinter( const AsyncCSVPrinter& other ){}

//AsyncCSVPrinter::AsyncCSVPrinter( AsyncCSVPrinter&& other ) {}

AsyncCSVPrinter::~AsyncCSVPrinter()
{
    {
        std::lock_guard<std::mutex> _lock(m_Mutex);
        m_running = false;
    }
    m_CV.notify_all();

    if( m_Thread.joinable() )
        m_Thread.join();
}

void AsyncCSVPrinter::printAll() const
{
    const auto& AllEntitys = m_SystemCore->getEntitys();
    for( size_t i = 0; i < AllEntitys.size(); i++ )
        m_Queue.push(AllEntitys[i].getEntityStateCopy());

}
void AsyncCSVPrinter::flush() const
{

}

void AsyncCSVPrinter::Run()
{
    std::unique_lock<std::mutex> _lock(m_Mutex);
    while( m_running )
    {
        m_CV.wait(_lock, [this](){
            return !m_Queue.empty() || !m_running;
        });

        _lock.unlock();

        while ( !m_Queue.empty() ) 
        {
            //Index problem
            //Print problem... Können es eigentlihc nciht mit dem Csv Printer machen funktioniert einfach nicht außer ich habe einen eigenen seperaten core was verrückt wäre
            auto State = m_Queue.try_pop().value();

        }

        _lock.lock();
    }
}
}


