#include "Interactions.h"
#include "Printer.h"
#include <System.h>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace Physik 
{
ClassicalSystem::ClassicalSystem() : m_Calculating (false), m_running(false)
{
    //INTEGRATOR
    m_Core = std::make_shared<ClassicalSystemCore>(std::make_unique<VelocityVerleit>());
    //PRINTER
    m_Printer = std::make_unique<AsyncCSVPrinter>(m_Core, "data.csv");

    //Becnhmarking
    m_TimeStart = std::chrono::steady_clock::now();
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

    //Becnhmarking
    m_TimeEnd = std::chrono::steady_clock::now();
    auto rutime = m_TimeEnd - m_TimeStart;
    double runtimrSec = std::chrono::duration<double>(rutime).count();
    std::cout << runtimrSec << "," << m_StepTime << "," << m_PrintTime << ","; 

    m_Core.reset();
    m_Printer.reset(nullptr);
}

void ClassicalSystem::Start() 
{
    {
        std::lock_guard<std::mutex> _lock ( m_Mutex );
        m_Calculating = true;
        m_running = true;
        m_Idle = false;
    }
    if( m_FinishedSet )
    {
        m_Finished = std::promise<void>();
        m_Future = m_Finished.get_future();
        m_FinishedSet = false;
    }

    if( !m_Thread.joinable() )
    {
        m_Finished = std::promise<void>();
        m_Future = m_Finished.get_future();
        m_Thread = std::thread([this](){ this->run(); }); 
    }

    m_SystemCV.notify_all();
}

bool ClassicalSystem::pauseInternal()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    const bool wasRunngin = m_Calculating;
    m_Calculating = false;
    m_IdleCV.wait(lock, [this]{ return m_Idle; });
    return wasRunngin;
}

void ClassicalSystem::Pause() 
{
    pauseInternal();
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
    PauseScope pause(*this);
    m_Core->addExternPotential( std::move(potential) );
    m_Core->UpdateEntityPropertys();
}

void ClassicalSystem::addMulitpleExternPotentials( std::vector<ClassicField> potentials )
{
    PauseScope pause(*this);
    m_Core->addMulitpleExternPotentials( std::move(potentials) );
    m_Core->UpdateEntityPropertys();
}

void ClassicalSystem::addEntityPotential( ClassicInteraction potential )
{
    PauseScope pause(*this);
    m_Core->addEntityPotential(std::move(potential));
    m_Core->UpdateEntityPropertys();
}

void ClassicalSystem::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials )
{
    PauseScope pause(*this);
    m_Core->addMultipleEntityPotentials( std::move(potentials) );
    m_Core->UpdateEntityPropertys();
}

void ClassicalSystem::addNonPotentialForce( ClassicNonPotentialForce NonPotForce ) 
{
    PauseScope pause(*this);
    m_Core->addNonPotentialForce(std::move(NonPotForce)); 
    m_Core->UpdateEntityPropertys();
}
void ClassicalSystem::addMultipleNonPotentialForce( std::vector<ClassicNonPotentialForce> NonPotForce ) 
{
    PauseScope pause(*this);
    m_Core->addMultipleNonPotentialForce(std::move(NonPotForce));
    m_Core->UpdateEntityPropertys();
}


void ClassicalSystem::addEntity(EntityDescription desc)
{
    PauseScope pause(*this);
    auto ID = m_Core->addEntity(std::move(desc));
    m_Core->UpdateEntityPropertys();

    auto& CreatedEntity = m_Core->getEntityRegister().getById(ID);
    m_Printer->OnEntityCreated(CreatedEntity);
}

void ClassicalSystem::addEntity( Vec3D Position, Vec3D Velocity, Rotation Rotation, Vec3D AngularVelocity, double Mass, Material<> Material, Shape<> Shape )
{
    PauseScope pause(*this);
    auto ID = m_Core->addEntity(Position, Velocity, Rotation, AngularVelocity, Mass, Material, Shape);
    m_Core->UpdateEntityPropertys();

    auto& CreatedEntity = m_Core->getEntityRegister().getById(ID);
    m_Printer->OnEntityCreated(CreatedEntity);
}

void ClassicalSystem::addMulipleEntitys( std::vector<EntityDescription> entitys )
{
    PauseScope pause(*this);
    auto IDs = m_Core->addMulipleEntitys( std::move(entitys) );
    m_Core->UpdateEntityPropertys();
    for( auto& ID : IDs )
    {
        auto& CreatedEntity = m_Core->getEntityRegister().getById(ID);
        m_Printer->OnEntityCreated(CreatedEntity);
    }
}

void ClassicalSystem::setTimeIncrement( double DeltaTime )
{
    PauseScope pause(*this);
    m_Core->setTimeIncrement( DeltaTime );
}
void ClassicalSystem::setTmax( double Tmax )
{
    assert(std::isfinite(Tmax));
    PauseScope pause(*this);
    m_Core->setTmax(Tmax);
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
        while( m_Calculating && m_Core->getTime() < m_Core->getTmax() )
            tick();

        _lock.lock();

        if(m_Core->getTime() >= m_Core->getTmax())
        {
            m_Calculating = false;
            if(!m_FinishedSet){ m_Finished.set_value(); m_FinishedSet = true; }
        }
        m_Idle = true;
        m_IdleCV.notify_all();
    }
    if(!m_FinishedSet){ m_Finished.set_value(); m_FinishedSet = true; }
    m_Idle = true;
    m_IdleCV.notify_all();
}

void ClassicalSystem::tick() 
{
    //Benchmarking
    auto t1 = std::chrono::steady_clock::now();
    m_Core->Step();
    auto t2 = std::chrono::steady_clock::now();
    auto d1 = t2-t1;
    m_StepTime += std::chrono::duration<double>(d1).count();

    auto t3 = std::chrono::steady_clock::now();
    m_Printer->Print();
    auto t4 = std::chrono::steady_clock::now();
    auto d2 = t4-t3;
    m_PrintTime += std::chrono::duration<double>(d2).count();
}
}
