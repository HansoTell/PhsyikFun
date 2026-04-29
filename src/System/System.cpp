#include "Entity.h"
#include "Interactions.h"
#include <System.h>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace Physik 
{
ClassicalSystem::ClassicalSystem() : m_running(false)
{
    m_Core = std::make_shared<ClassicalSystemCore>();
    m_Printer = std::make_unique<ConsolePrinter>(m_Core);
}

ClassicalSystem::ClassicalSystem( std::unique_ptr<IPrinter> printer) : m_Printer(std::move(printer)) ,m_running(false) {}

ClassicalSystem::ClassicalSystem( const ClassicalSystem& other ) 
{
    m_Core = std::make_shared<ClassicalSystemCore>(*other.m_Core);
    m_Printer = other.m_Printer->clone(m_Core);
}

ClassicalSystem::~ClassicalSystem() 
{
    Pause();
    if( m_Thread.joinable() )
        m_Thread.join();

    m_Core.reset();
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

void ClassicalSystem::addExternPotential( ClassicField potential )
{
    Pause();
    m_Core->addExternPotential( std::move(potential) );
    Start();
}

void ClassicalSystem::addMulitpleExternPotentials( std::vector<ClassicField> potentials )
{
    Pause();
    m_Core->addMulitpleExternPotentials( std::move(potentials) );
    Start();
}

void ClassicalSystem::addEntityPotential( ClassicInteraction potential )
{
    Pause();
    m_Core->addEntityPotential(std::move(potential));
    Start();
}

void ClassicalSystem::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials )
{
    Pause();
    m_Core->addMultipleEntityPotentials( std::move(potentials) );
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
    m_Printer->printEntityPositions();
    while( m_running )
        tick();
}

void ClassicalSystem::tick() 
{
    m_Core->moveEntitys();

    m_Core->advanceTimeIncrement();

    m_Printer->printEntityPositions();
}
}
