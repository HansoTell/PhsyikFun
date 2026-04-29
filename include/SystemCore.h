#pragma once

#include "Interactions.h"
#include "Entity.h"
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
    Vector<Dim, T> deltaVelocity;
    Vector<Dim, T> deltaPosition;
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
    //vorläufig
    void addEntityPotential( ClassicInteraction potential);
    void addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials );

    void advanceTimeIncrement();
    void moveEntitys();

    const std::vector<ClassicEntity>& getEntitys() const { return m_Entitys; }

public:
    ClassicalSystemCore();
    ClassicalSystemCore( double DeltaTime );
    ClassicalSystemCore(const ClassicalSystemCore& other);
    ClassicalSystemCore( ClassicalSystemCore&& other);
    ~ClassicalSystemCore() = default;
private:
    std::vector<ClassicEntityPropertys> ClacEffektOfPotentials() const;
    ClassicEntityPropertys CalcEffectOnEntity( const ClassicEntity& entitys, size_t idx ) const;
    Vec3D CalcForceOfExtPotentials( const std::vector<ClassicField>& potentials, const ClassicEntity& entity ) const;
    Vec3D CalcForceOfEntityPotentials( const std::vector<ClassicInteraction>& potentials, const ClassicEntity& ent1, const ClassicEntity& ent2) const;
    void ApplyMovementOnEntitys( const std::vector<ClassicEntityPropertys>& Propertys );
private:
    std::vector<ClassicEntity> m_Entitys;
    std::vector<ClassicField> m_ExtPotentials;
    std::vector<ClassicInteraction> m_EntityPotentials;

    double m_Time;
    double m_DeltaTime;
    double Energy;
};

}
