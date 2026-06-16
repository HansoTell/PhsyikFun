#pragma once

#include "Entity.h"
#include "Vector.h"
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
}
