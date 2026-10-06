#pragma once

#include "Interactions.h"
#include "Material.h"
#include "Printer.h"
#include "Shapes.h"
#include "SystemCore.h"
#include "Vector.h"
#include "EntityDescribtion.h"

#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#define CREATE_CLASSIC_ENTITY__STANDART_POTENTIAL( beta ) ClassicInteraction( std::make_unique<ClassicStandartPotential>(beta) )
#define CREATE_CLASSIC_ENTITY_GRAVITATIONAL_POTENTIAL() ClassicInteraction( std::make_unique<ClassicGravitationPotential>() ) 

namespace Physik 
{

using Rotation = Quaternion<>;
inline Rotation CreateRotation( double Angel, Vec3D Axis ) { return Quaternion<>::FromAxisAngle(Angel, Axis); }
inline Rotation CombineRotation( const Rotation& first, const Rotation& secound ) { auto res = first * secound; res.NormQuaterion(); return res; }

inline const Rotation NoRotation = CreateRotation(0.0, Vec3D{1.0, 0.0, 0.0});

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
    void addNonPotentialForce( ClassicNonPotentialForce NonPotForce );
    void addMultipleNonPotentialForce( std::vector<ClassicNonPotentialForce> NonPotForce );
    void addEntity(EntityDescription desc);
    void addEntity( Vec3D Position, Vec3D Velocity, Rotation Rotation, Vec3D AngularVelocity, double Mass, Material<> Material, Shape<> Shape );
    void addMulipleEntitys( std::vector<EntityDescription> entitys );
    void setTimeIncrement( double DeltaTime ); 
    void setTmax( double Tmax ); 
    bool isRunning() const { return m_running; }

    const std::future<void>& getFuture() const { return m_Future; }
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
    std::promise<void> m_Finished;
    std::future<void> m_Future;
    std::mutex m_Mutex;
    std::condition_variable m_SystemCV;
    bool m_Calculating;
    std::atomic<bool> m_running;
};

}
