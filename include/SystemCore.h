#pragma once

#include "Entity.h"

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
    void addPotential( std::unique_ptr<ClassicPotential> potential );
    void addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials );
    void addEntity( ClassicEntity entity );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );
    void setTimeIncrement( double DeltaTime ) { m_DeltaTime = DeltaTime; }

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
    void CalcForceOfPotentialsOnEntity( const std::vector<std::unique_ptr<IPotential<3, double>>>& potentials, const ClassicEntity& entity, ClassicEntityPropertys& outPropertys ) const;
    void ApplyMovementOnEntitys( const std::vector<ClassicEntityPropertys>& Propertys );
private:
    std::vector<ClassicEntity> m_Entitys;
    std::vector<std::unique_ptr<ClassicPotential>> m_ExtPotentials;

    double m_Time;
    double m_DeltaTime;
    double Energy;
};

}
