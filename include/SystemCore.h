#pragma once

#include "Interactions.h"
#include "Entity.h"
#include "PropertyCalculus.h"
#include <memory>
#include <vector>

#define DEFAULT_DELTA_TIME 0.001

namespace Physik 
{
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

    void UpdateEntityPropertys();
    void UpdateEnergy();

    void MakeTimeStep();

    const std::vector<ClassicEntity>& getEntitys() const { return m_Entitys; }
    double getEnergy() const { return Energy; }

public:
    ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> PropertyCalcer );
    ClassicalSystemCore( std::unique_ptr<IPropertyCalculus> dglMethod, double DeltaTime );
    ClassicalSystemCore(const ClassicalSystemCore& other);
    ClassicalSystemCore( ClassicalSystemCore&& other);
    ~ClassicalSystemCore() = default;
private:
    void advanceTimeIncrement();
    std::vector<ClassicEntityState> ClacNewEntityStates() const;
    void ApplyAllForces( std::vector<ClassicEntityState>& outPropertys ) const;
    void ApplyAllEnergy( std::vector<ClassicEntityState>& outPropertys ) const;
    void ApplyNewEntityStates( const std::vector<ClassicEntityState>& Propertys );
private:
    std::vector<ClassicEntity> m_Entitys;
    std::vector<ClassicField> m_ExtPotentials;
    std::vector<ClassicInteraction> m_EntityPotentials;

    double m_Time;
    double m_DeltaTime;
    double Energy;
    
    std::unique_ptr<IPropertyCalculus> m_PropertyCalcer;
};
}
