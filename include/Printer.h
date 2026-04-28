#pragma once

#include "SystemCore.h"

#include <memory>

namespace Physik 
{
//geben wir die gebrauchten propertys zum printen in der konstruktor hinzu???
class IPrinter 
{
public:
    ~IPrinter() = default;
    virtual void printEntityPositions() const = 0;
    virtual std::unique_ptr<IPrinter> clone( std::shared_ptr<const ClassicalSystemCore> newCore ) const = 0;
};

class ConsolePrinter : public IPrinter 
{
public:
    void printEntityPositions() const override; 

    std::unique_ptr<IPrinter> clone( std::shared_ptr<const ClassicalSystemCore> newCore ) const override { return std::make_unique<ConsolePrinter>(newCore); }
public:
    ConsolePrinter( const std::shared_ptr<const ClassicalSystemCore> SystemCore );
    ConsolePrinter( const ConsolePrinter& other );
    ConsolePrinter( ConsolePrinter&& other );
    ~ConsolePrinter() = default;
private:
    std::shared_ptr<const ClassicalSystemCore> m_SystemCore;
};
    
}
