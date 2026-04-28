#include "Entity.h"
#include <System.h>
#include <memory>
#include <mutex>
#include <thread>


namespace Physik 
{
ClassicalSystem::ClassicalSystem() : m_running(false)
{
    m_Printer = std::make_unique<ConsolePrinter>();
    m_Core = std::make_unique<ClassicalSystemCore>();
}

ClassicalSystem::ClassicalSystem( std::unique_ptr<IPrinter> printer) : m_Printer(std::move(printer)) ,m_running(false) {}

ClassicalSystem::ClassicalSystem( const ClassicalSystem& other ) 
{
    m_Printer = other.m_Printer->clone();
    m_Core = std::make_unique<ClassicalSystemCore>(*other.m_Core);
}

ClassicalSystem::~ClassicalSystem() 
{
    Pause();
    if( m_Thread.joinable() )
        m_Thread.join();

    m_Core.reset(nullptr);
    m_Printer.reset(nullptr);
}

void ClassicalSystem::Start() 
{
    {
        std::lock_guard<std::mutex> _lock ( m_Mutex );
        m_running = true;
    }

    if( !m_Thread.joinable() )
        m_Thread = std::thread([this](){ this->run(); }); 
}

void ClassicalSystem::Pause() 
{
    std::lock_guard<std::mutex> _lock( m_Mutex );
    m_running = false;
}

void ClassicalSystem::Clear()
{
    Pause();
    if( m_Thread.joinable() )
        m_Thread.join();

    m_Core->Clear();
}

void ClassicalSystem::addPotential( std::unique_ptr<ClassicPotential> potential )
{
    Pause();
    m_Core->addPotential( std::move(potential) );
    Start();
}

void ClassicalSystem::addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials )
{
    Pause();
    m_Core->addMulitpPotentials( std::move(potentials) );
    Start();
}

void ClassicalSystem::addEntity( ClassicEntity entity )
{
    Pause();
    m_Core->addEntity( std::move(entity) );
    Start();
}

void ClassicalSystem::addMulipleEntitys( std::vector<ClassicEntity> entitys )
{
    Pause();
    m_Core->addMulipleEntitys( std::move(entitys) );
    Start();
}

void ClassicalSystem::setTimeIncrement( double DeltaTime )
{
    Pause();
    m_Core->setTimeIncrement( DeltaTime );
    Start();
}

void ClassicalSystem::run() 
{
    while( m_running )
        tick();
}

void ClassicalSystem::tick() 
{

}
}
