#include "Entity.h"
#include "Printer.h"
#include <cstddef>
#include <optional>

namespace Physik 
{

AsyncCSVPrinter::AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath ) 
    : m_Queue(), m_SystemCore(SystemCore), m_running(true), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(FilePath)))
{
    m_Thread = std::thread([this](){ this->Run(); });
}
AsyncCSVPrinter::AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath, PrintOptions options ) :
    m_SystemCore(SystemCore), m_running(true), m_FileWriter(std::make_unique<CSVFileWriter>(std::move(FilePath), options))
{
    m_Thread = std::thread([this](){ this->Run(); });
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

void AsyncCSVPrinter::Print() const
{
    const auto& AllEntitys = m_SystemCore->getEntityRegister();
    for( size_t i = 0; i < AllEntitys.size(); i++ )
        m_Queue.push( { AllEntitys[i], m_SystemCore->getTime() } );

    m_CV.notify_all();
}

void AsyncCSVPrinter::OnEntityCreated(const ClassicEntity& Entity) const 
{ 
    m_ConstantsQueue.push({ Entity.getID(), Entity.getConstants(), Entity.getMaterial(), Entity.getShape() }); 
    m_CV.notify_all();
}

void AsyncCSVPrinter::Run()
{
    constexpr size_t RepetitionLimit = 500;

    std::unique_lock<std::mutex> _lock(m_Mutex);
    while( m_running )
    {
        m_CV.wait(_lock, [this](){
            return !m_Queue.empty() || !m_running;
        });

        _lock.unlock();

        while(!m_Queue.empty() || !m_ConstantsQueue.empty())
        {
            for( size_t i = 0; i < RepetitionLimit; ++i )
            {
                auto EntInfo_or = m_Queue.try_pop();
                if(!EntInfo_or)
                    break;

                m_FileWriter->WriteState(EntInfo_or->State, EntInfo_or->Time);
            }

            for(size_t i = 0; i < RepetitionLimit; ++i )
            {
                auto Constant_or = m_ConstantsQueue.try_pop();
                if(!Constant_or)
                    break;

                m_FileWriter->WirteConstantPropertys(*Constant_or);
            }
        }

        _lock.lock();
    }
    m_FileWriter->flush();
}
}
