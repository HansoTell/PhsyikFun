#pragma once

#include "Datastructures/ThreadSaveQueue.h"
#include "Entity.h"
#include "SystemCore.h"
#include "Vector.h"

#include <atomic>
#include <charconv>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>

//TODO anpassen alles so dass es unterschieldich3e formate unterstützt
namespace Physik 
{

struct ClassicEntityInfo
{
    uint64_t EntityID;
    ClassicEntityState State;
    double Time;
};

enum class PrintOptions : uint16_t 
{
    eNone = 0b0,
    ePosition = 0b1,
    eVelocity = 0b10,
    eAcceleration = 0b100,
    eForce = 0b1000,
    eKinEnergy = 0b10000,
    ePotEnergy = 0b100000,
    eAll = 0b111111
};

constexpr PrintOptions operator|( PrintOptions a, PrintOptions b ) { return static_cast<PrintOptions>( static_cast<uint16_t>(a) | static_cast<uint16_t>(b) ); }
constexpr PrintOptions operator&( PrintOptions a, PrintOptions b ) { return static_cast<PrintOptions>( static_cast<uint16_t>(a) & static_cast<uint16_t>(b) ); }
constexpr bool has( PrintOptions set, PrintOptions flag ) { return (set & flag) != PrintOptions::eNone; }


class IPrinter 
{
public:
    virtual ~IPrinter() = default;
    virtual void Print() const = 0;
};

class ConsolePrinter : public IPrinter 
{
public:
    void Print() const override;

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
    void WriteState( const ClassicEntityState& state, uint64_t ID, double t ) const;
    void flush() const;
public:
    CSVFileWriter( std::string FilePath );
    CSVFileWriter( std::string FilePath, PrintOptions options );
    CSVFileWriter( const CSVFileWriter& other ) = delete;
    CSVFileWriter( CSVFileWriter&& other ) = delete;
    ~CSVFileWriter() = default;
private:
    void printEntityStateHeader() const;
    void PrintVector( const Vec3D& vector ) const;
    template<typename T>
    void PrintNumber( T number ) const
    {
        static_assert(std::is_arithmetic_v<T>);

        char buffer[128];
        auto[end, erc] = std::to_chars(buffer, buffer+sizeof(buffer), number);

        if( erc != std::errc() )
        {
            std::cout << "to chars filed\n";
            return;
        }
        m_Buffer.append(buffer, end);
    }
    void PrintSeperator() const;
    void PrintLineEnd() const;
private:
    PrintOptions m_Options;

    mutable std::ofstream m_File;
    std::string m_FilePath;
    mutable std::string m_Buffer;
};

class CSVPrinter : public IPrinter 
{
public:
    void Print() const override;
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
    std::atomic<bool> m_running;
    std::mutex m_Mutex;
    mutable std::condition_variable m_CV;
    std::thread m_Thread;
};
    
}
