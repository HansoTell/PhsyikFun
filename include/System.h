#pragma once

#include "Entity.h"
#include "Interactions.h"
#include "Printer.h"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#define CREATE_CLASSIC_EXT_STANDART_POTENTIAL( beta, xPos, yPos, zPos ) ClassicField( std::make_unique<ClassicStandartPotential>(beta),  ClassicEntityState(Vec3D{ xPos, yPos, zPos }, Vec3D{ 0.0, 0.0, 0.0 }, 0.0 ) ) 
#define CREATE_CLASSIC_ENTITY__STANDART_POTENTIAL( beta ) ClassicInteraction( std::make_unique<ClassicStandartPotential>(beta) )
#define CREATE_CLASSIC_EXT_GRAVITATIONAL_POTENTIAL( Mass, xPos, yPos, zPos ) ClassicField( std::make_unique<ClassicGravitationPotential>(), ClassicEntityState(Vec3D{ xPos, yPos, zPos }, Vec3D{ 0.0, 0.0, 0.0 }, Mass) ) 
#define CREATE_CLASSIC_ENTITY_GRAVITATIONAL_POTENTIAL() ClassicInteraction( std::make_unique<ClassicGravitationPotential>() ) 

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
    void addExternPotential( ClassicField potential );
    void addEntityPotential( ClassicInteraction potential );
    void addMulitpleExternPotentials( std::vector<ClassicField> potentials );
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials );
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
