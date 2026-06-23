#pragma once

#include "Entity.h"
#include "Vector.h"
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace Physik
{
using SimulationState = std::vector<ClassicEntity>;


class IImpactEvaluator  
{
public:
    virtual ~IImpactEvaluator() = default;
    virtual void ApplyImpacts( SimulationState& state ) = 0;
};

class ElasticImpact : public IImpactEvaluator 
{
public:
    void ApplyImpacts( SimulationState& state ) override;
public:
    ElasticImpact() = default;
    ElasticImpact( const ElasticImpact& other ) = default;
    ElasticImpact( ElasticImpact&& other ) = default;
    ~ElasticImpact() = default;
private:
    Vec3D CalcVeclocityNormal( const Vec3D& veclocity, const Vec3D& normal ) const;
    Vec3D CalcVelocityTangential( const Vec3D& veclocity, const Vec3D& normal ) const;
    double CalcVelocAfter( const ClassicEntity& target, const Vec3D& targetNormal, const ClassicEntity& other, const Vec3D& otherNormal ) const;
};

class AdvancedElasticImpact : public IImpactEvaluator 
{
    struct StateKoords
    {
        int32_t x_Koord, y_Koord, z_Koord;
    };
    struct StateKoordsHash
    {
        std::size_t operator() ( const StateKoords& key ) const 
        {
            return 1;
        }
    };
public:
    void ApplyImpacts( SimulationState& state );
public:
    AdvancedElasticImpact( const std::vector<const ClassicEntity>& entitys ); 
    AdvancedElasticImpact( const AdvancedElasticImpact& ) = default;
    AdvancedElasticImpact( AdvancedElasticImpact&& ) = default;
    ~AdvancedElasticImpact() = default;
private:
    const std::vector<const ClassicEntity>& m_Entitys;
    std::map<StateKoords, ClassicEntity*> m_Map;
}; 
}
