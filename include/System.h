#pragma once

#include "Entity.h"
#include "Printer.h"
#include "Potential.h"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


//idee für später: Einen allgemeinen system thread machen und dann kann man sein systemcore rein geben als dependeciy dann rbaucht man kein thread je nach system
namespace Physik 
{



class ISystem 
{
public: 
    ~ISystem() = default;
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Clear() = 0;
};


class ClassicalSystem : public ISystem 
{
public:
    void Start() override;
    void Pause() override;
    void Clear() override;
    void addPotential( std::unique_ptr<ClassicIPotential> potential );
    void addMulitpPotentials( std::vector<std::unique_ptr<ClassicIPotential>> potentials );
    void addEntity( ClassicEntity entity );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );
    void setTimeIncrement( double DeltaTime ); 

public:
    //more Konstruktores
    ClassicalSystem();
    ClassicalSystem( std::unique_ptr<IPrinter> printer );
    ClassicalSystem(const ClassicalSystem& other);
    ClassicalSystem(ClassicalSystem&& other) = delete;
    ~ClassicalSystem();
private:
    void run();
    void tick();
private:
    std::unique_ptr<IPrinter> m_Printer;
    std::shared_ptr<ClassicalSystemCore> m_Core;

    std::thread m_Thread;
    std::mutex m_Mutex;
    bool m_running;
};
}
