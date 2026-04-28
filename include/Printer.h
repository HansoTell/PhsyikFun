#pragma once

#include <memory>

namespace Physik 
{
class IPrinter 
{
public:
    ~IPrinter() = default;
    virtual std::unique_ptr<IPrinter> clone() const = 0;
};

class ConsolePrinter : public IPrinter 
{
public:
    std::unique_ptr<IPrinter> clone() const override { return std::make_unique<ConsolePrinter>(); }

private:

};
    
}
