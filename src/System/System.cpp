#include "Entity.h"
#include "Interactions.h"
#include "Printer.h"
#include "PropertyCalculus.h"
#include <System.h>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace Physik 
{
ClassicalSystem::ClassicalSystem() : m_Calculating (false), m_running(false)
{
    m_Core = std::make_shared<ClassicalSystemCore>(std::make_unique<ClassicPropCalc>(std::make_unique<VelocityVerleit>()));
    //PRINTER
    m_Printer = std::make_unique<AsyncCSVPrinter>(m_Core, "data.csv");
}

ClassicalSystem::ClassicalSystem( std::unique_ptr<IPrinter> printer) : m_Printer(std::move(printer)) ,m_Calculating (false), m_running(false) {}

ClassicalSystem::ClassicalSystem( const ClassicalSystem& other ) 
{
    m_Core = std::make_shared<ClassicalSystemCore>(*other.m_Core);
    //m_Printer = other.m_Printer->clone();
}

ClassicalSystem::~ClassicalSystem() 
{
    Clear();

    m_Core.reset();
    m_Printer.reset(nullptr);
}

void ClassicalSystem::Start() 
{
    {
        std::lock_guard<std::mutex> _lock ( m_Mutex );
        m_Calculating = true;
        m_running = true;
    }

    if( !m_Thread.joinable() )
        m_Thread = std::thread([this](){ this->run(); }); 

    m_SystemCV.notify_all();
}

void ClassicalSystem::Pause() 
{
    std::lock_guard<std::mutex> _lock( m_Mutex );
    m_Calculating = false;

    //kann man iwie garantieren dass hier aufgehalten wird bis er in cv gelaufen ist?
}

void ClassicalSystem::Clear()
{
    {
        std::lock_guard<std::mutex> _lock( m_Mutex );
        m_running = false;
        m_Calculating = false;
    }
    m_SystemCV.notify_all();

    if( m_Thread.joinable() )
        m_Thread.join();

    m_Core->Clear();
}

void ClassicalSystem::addExternPotential( ClassicField potential )
{
    Pause();
    m_Core->addExternPotential( std::move(potential) );
    m_Core->UpdateEntityPropertys();
    Start();
}

void ClassicalSystem::addMulitpleExternPotentials( std::vector<ClassicField> potentials )
{
    Pause();
    m_Core->addMulitpleExternPotentials( std::move(potentials) );
    m_Core->UpdateEntityPropertys();
    Start();
}

void ClassicalSystem::addEntityPotential( ClassicInteraction potential )
{
    Pause();
    m_Core->addEntityPotential(std::move(potential));
    m_Core->UpdateEntityPropertys();
    Start();
}

void ClassicalSystem::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials )
{
    Pause();
    m_Core->addMultipleEntityPotentials( std::move(potentials) );
    m_Core->UpdateEntityPropertys();
    Start();
}

void ClassicalSystem::addEntity( ClassicEntity entity )
{
    Pause();
    m_Core->addEntity( std::move(entity) );
    m_Core->UpdateEntityPropertys();
    Start();
}

void ClassicalSystem::addMulipleEntitys( std::vector<ClassicEntity> entitys )
{
    Pause();
    m_Core->UpdateEntityPropertys();
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
    m_Printer->Print();
    std::unique_lock<std::mutex> _lock(m_Mutex);
    while( m_running )
    {
        m_SystemCV.wait(_lock, [this](){
            return m_Calculating || !m_running;
        });

        if( !m_running )
            break;

        _lock.unlock();
        while( m_Calculating )
            tick();

        _lock.lock();
    }
}

void ClassicalSystem::tick() 
{
    m_Core->MakeTimeStep();

    m_Printer->Print();
}
}
