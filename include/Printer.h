#pragma once

#include "SystemCore.h"

#include <fstream>
#include <iostream>
#include <ios>
#include <memory>

namespace Physik 
{
class IPrinter 
{
public:
    ~IPrinter() = default;
    virtual void printEntityPositions() const = 0;
    virtual void printForce() const = 0;
    virtual void printEnergy() const = 0;
    virtual std::unique_ptr<IPrinter> clone() const = 0;
};

class ConsolePrinter : public IPrinter 
{
public:
    void printEntityPositions() const override; 
    void printEnergy() const override;
    void printForce() const override;

    std::unique_ptr<IPrinter> clone() const override { return std::make_unique<ConsolePrinter>(*this); }
public:
    ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore );
    ConsolePrinter( const ConsolePrinter& other );
    ConsolePrinter( ConsolePrinter&& other );
    ~ConsolePrinter() = default;
private:
    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};

class FilePrinter : public IPrinter 
{
public:
    void printEntityPositions() const override; 
    void printEnergy() const override;
    void printForce() const override;
    std::unique_ptr<IPrinter> clone() const override { return std::make_unique<FilePrinter>(*this); }
public:
    FilePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore, std::string filepath );
    FilePrinter( const FilePrinter& other );
    FilePrinter( FilePrinter&& other ); 
    ~FilePrinter() 
    { 
        m_File.flush();
        if( m_File.is_open() ) m_File.close(); 
    } 
private:
    mutable std::ofstream m_File;
    std::string m_FilePath;

    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};
    
}
