#include "Printer.h"
#include <cstdint>


namespace Physik 
{

AsyncCSVPrinter::AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath ) 
    : m_Queue(), m_SystemCore(SystemCore), m_running(true), counter(0), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(FilePath)))
{
    m_Thread = std::thread([this](){ this->Run(); });
}
AsyncCSVPrinter::AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath, std::initializer_list<PrintOptions> options ) :
    m_SystemCore(SystemCore), m_running(true), counter(0), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(FilePath))), m_Options(0)
{
    for( PrintOptions po : options )
        m_Options &= (uint16_t) po;
}

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
        m_Queue.push( { AllEntitys[i].getID(), AllEntitys[i].getEntityStateCopy() } );

    m_CV.notify_all();
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

        while (auto EntityInfo = m_Queue.try_pop()) 
        {
            //wie kann man es richtig machen die zeile dann auhc zu beeden entweder wir gebe die optionen weiter und managen unten was aber nicht gut wäre
            //--> ich denke einfach öffentlich das print seperator und print end machen oder so, also alles nach ußen exposen
            m_FileWriter->WriteState(EntityInfo->State, EntityInfo->EntityID);

        }


        _lock.lock();
    }
    m_FileWriter->flush();
}
}
