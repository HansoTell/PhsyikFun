#pragma once

#include "Datastructures/ThreadSaveQueue.h"
#include "Entity.h"
#include "EntityRegistry.h"
#include "Material.h"
#include "Shapes.h"
#include "SystemCore.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Physik 
{
struct ClassicEntityInfo
{
    ClassicEntity State;
    double Time;
};

struct ClassicConstantsInfo
{
    EntityRegistry::ID id;
    ConstantPrtopertys<> constants;
    Material<> material;
    Shape<> shape;
};

enum class PrintOptions : uint16_t 
{
    eNone =                 0b0,
    ePosition =             0b1,
    eVelocity =             0b10,
    eAcceleration =         0b100,
    eRotation =             0b1000,
    eAngularVelocity =      0b10000,
    eAngularAcceleration =  0b100000,
    eForce =                0b1000000,
    eKinEnergy =            0b10000000,
    ePotEnergy =            0b100000000,
    eConstants =            0b1000000000,
    eAll =                  0b1111111111
};

constexpr PrintOptions operator|( PrintOptions a, PrintOptions b ) { return static_cast<PrintOptions>( static_cast<uint16_t>(a) | static_cast<uint16_t>(b) ); }
constexpr PrintOptions operator&( PrintOptions a, PrintOptions b ) { return static_cast<PrintOptions>( static_cast<uint16_t>(a) & static_cast<uint16_t>(b) ); }
constexpr bool has( PrintOptions set, PrintOptions flag ) { return (set & flag) != PrintOptions::eNone; }


class IPrinter 
{
public:
    virtual ~IPrinter() = default;
    virtual void Print() const = 0;
    virtual void OnEntityCreated(const ClassicEntity&) const = 0;
};

class ConsolePrinter : public IPrinter 
{
public:
    void Print() const override;
    void OnEntityCreated(const ClassicEntity&) const override {}

public:
    ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore );
    ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, PrintOptions options );
    ConsolePrinter( const ConsolePrinter& other ) = delete;
    ConsolePrinter( ConsolePrinter&& other ) = delete;
    ~ConsolePrinter() = default;
private:
    void printPosition() const; 
    void printVelocity() const;
    void printAcceleration() const;
    void printKineticEnergy() const;
    void printPotentialEnergy() const;
    void printForce() const;
private:
    PrintOptions m_Options;
    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};

class CSVFileWriter 
{
public:
    void WriteState( const ClassicEntity& state, double t ) const;
    //TODO:  -> wie ist es mit flush
    void WirteConstantPropertys( const ClassicConstantsInfo& state ) const;
    void flush() const;
public:
    CSVFileWriter( std::string FilePath );
    CSVFileWriter( std::string FilePath, PrintOptions options );
    CSVFileWriter( const CSVFileWriter& other ) = delete;
    CSVFileWriter( CSVFileWriter&& other ) = delete;
    ~CSVFileWriter() = default;
private:
    void printEntityStateHeader() const;
    void printConstantsHeader() const;

    void flushVariableFile() const;
    void flushConstantFile() const;
private:
    PrintOptions m_Options;

    mutable std::ofstream m_File;
    mutable std::ofstream m_ConstantsFile;
    std::string m_FilePath;
    mutable std::string m_Buffer;
    mutable std::string m_ConstanstBuffer;
};

class CSVPrinter : public IPrinter 
{
public:
    void Print() const override;
    void OnEntityCreated(const ClassicEntity& entity) const override { m_FileWriter->WirteConstantPropertys({entity.getID(), entity.getConstants(), entity.getMaterial(), entity.getShape()}); }
public:
    CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath );
    CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath, PrintOptions options );
    CSVPrinter( const CSVPrinter& other ) = delete;
    CSVPrinter( CSVPrinter&& other ) = delete;
    ~CSVPrinter() { m_FileWriter->flush(); } 
private:
    std::unique_ptr<CSVFileWriter> m_FileWriter;
    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};

class AsyncCSVPrinter : public IPrinter 
{
public:
    void Print() const override;
    void OnEntityCreated(const ClassicEntity& entity) const override;
public:
    AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath );
    AsyncCSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string FilePath, PrintOptions options );
    AsyncCSVPrinter( const AsyncCSVPrinter& other ) = delete;
    AsyncCSVPrinter( AsyncCSVPrinter&& other ) = delete;
    ~AsyncCSVPrinter();
private:
    void Run();
private:
    std::unique_ptr<CSVFileWriter> m_FileWriter;
    const std::shared_ptr<const ClassicalSystemCore> m_SystemCore;

    mutable http::ThreadSaveQueue<ClassicEntityInfo> m_Queue;
    mutable http::ThreadSaveQueue<ClassicConstantsInfo> m_ConstantsQueue;
    std::atomic<bool> m_running;
    std::mutex m_Mutex;
    mutable std::condition_variable m_CV;
    std::thread m_Thread;
};
    
}
