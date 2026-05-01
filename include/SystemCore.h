#pragma once

#include "Interactions.h"
#include "Entity.h"
#include "NumericNewontDGLSolver.h"
#include <memory>
#include <vector>

#define DEFAULT_DELTA_TIME 0.001

namespace Physik 
{
template<size_t Dim = 3, typename T = double> 
struct EntityPropertys 
{
    size_t EntityIndex;
    Vector<Dim, T> Force;
    Vector<Dim, T> Acceleration;
    Vector<Dim, T> Velocity;
    Vector<Dim, T> Position;
    T KinEnergy;
    T PotEnergy;
};

using ClassicEntityPropertys = EntityPropertys<3, double>;

class ClassicalSystemCore 
{
public:
    void Clear();
    void addExternPotential( ClassicField potential );
    void addMulitpleExternPotentials( std::vector<ClassicField> potentials );
    void addEntity( ClassicEntity entity );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );
    void setTimeIncrement( double DeltaTime ) { m_DeltaTime = DeltaTime; }
    void addEntityPotential( ClassicInteraction potential);
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials );

    void advanceTimeIncrement();
    void moveEntitys();

    const std::vector<ClassicEntity>& getEntitys() const { return m_Entitys; }

public:
    ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod );
    ClassicalSystemCore( std::unique_ptr<IDGLSolver> dglMethod, double DeltaTime );
    ClassicalSystemCore(const ClassicalSystemCore& other);
    ClassicalSystemCore( ClassicalSystemCore&& other);
    ~ClassicalSystemCore() = default;
private:
    std::vector<ClassicEntityPropertys> ClacEffektOfPotentials() const;
    void CalcEffectOnEntity( const ClassicEntity& entitys, size_t idx, std::vector<ClassicEntityPropertys>& outPropertys ) const;
    Vec3D CalcForceOfExtPotentials( const std::vector<ClassicField>& potentials, const ClassicEntity& entity ) const;
    Vec3D CalcForceOfEntityPotentials( const std::vector<ClassicInteraction>& potentials, const ClassicEntity& ent1, const ClassicEntity& ent2 ) const;
    double CalcPotEnergyOfEntityPotentials ( const std::vector<ClassicInteraction>& potentials, const ClassicEntity& ent1, const ClassicEntity& ent2 ) const;
    double CalcPotEnergyOfExtPotentials( const std::vector<ClassicField>& potentials, const ClassicEntity& entity ) const;
    void ApplyMovementOnEntitys( const std::vector<ClassicEntityPropertys>& Propertys );
private:
    std::vector<ClassicEntity> m_Entitys;
    std::vector<ClassicField> m_ExtPotentials;
    std::vector<ClassicInteraction> m_EntityPotentials;

    double m_Time;
    double m_DeltaTime;
    double Energy;

    std::unique_ptr<IDGLSolver> m_DGLMethod;
};

}
