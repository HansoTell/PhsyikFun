#pragma once

#include "SystemCore.h"
#include "Vector.h"

#include <cstdint>
#include <fstream>
#include <memory>

//TODO anpassen alles so dass es unterschieldich3e formate unterstützt
namespace Physik 
{
class IPrinter 
{
public:
    ~IPrinter() = default;
    virtual void printEntityPositions() const = 0;
    virtual void printForce() const = 0;
    virtual void printEnergy() const = 0;
    virtual void printAll() const = 0;
    virtual void flush() const = 0;
    virtual std::unique_ptr<IPrinter> clone() const = 0;
};

class ConsolePrinter : public IPrinter 
{
public:
    void printEntityPositions() const override; 
    void printEnergy() const override;
    void printForce() const override;
    void printAll() const override;
    void flush() const override {}

    std::unique_ptr<IPrinter> clone() const override { return std::make_unique<ConsolePrinter>(*this); }
public:
    ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore );
    ConsolePrinter( const ConsolePrinter& other );
    ConsolePrinter( ConsolePrinter&& other );
    ~ConsolePrinter() = default;
private:
    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};

class CSVPrinter : public IPrinter 
{
public:
    void printEntityPositions() const override; 
    void printEnergy() const override;
    void printForce() const override;
    void printAll() const override;
    void flush() const override;
    std::unique_ptr<IPrinter> clone() const override { return std::make_unique<CSVPrinter>(*this); }
public:
    CSVPrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath );
    CSVPrinter( const CSVPrinter& other );
    CSVPrinter( CSVPrinter&& other ); 
    ~CSVPrinter() 
    { 
        m_File.flush();
        if( m_File.is_open() ) m_File.close(); 
    } 
private:
    void printEntityStateHeader() const;
    void PrintVector( const Vec3D& vector ) const;
    void PrintSeperator() const;
    void PrintLineEnd() const; 
private:
    mutable std::ofstream m_File;
    std::string m_FilePath;
    mutable std::string m_Buffer;
    mutable uint32_t counter;

    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};
    
}
