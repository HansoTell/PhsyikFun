#pragma once

#include "Entity.h"
#include "Vector.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>
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
    struct CellKoords
    {
        int32_t x_Koord, y_Koord, z_Koord;
        bool operator == ( const CellKoords& other ) const { return  this->x_Koord == other.x_Koord && this->y_Koord == other.y_Koord && this->z_Koord == other.z_Koord; }
        static CellKoords getCell( const Vec3D& pos ) 
        { 
            return { 
                static_cast<int32_t>(std::floor(pos[0]/1.0)), 
                static_cast<int32_t>(std::floor(pos[1]/1.0)), 
                static_cast<int32_t>(std::floor(pos[2]/1.0)) 
            }; 
        }
    };
    struct CellHash 
    {
        std::size_t operator() (const CellKoords& c) const 
        {
            size_t h1 = std::hash<int32_t>{}(c.x_Koord);
            size_t h2 = std::hash<int32_t>{}(c.y_Koord);
            size_t h3 = std::hash<int32_t>{}(c.z_Koord);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
    static const std::array<CellKoords, 25> offsets;
public:
    void ApplyImpacts( SimulationState& state );
public:
    AdvancedElasticImpact(  std::vector<ClassicEntity>& entitys ); 
    AdvancedElasticImpact( const AdvancedElasticImpact& ) = default;
    AdvancedElasticImpact( AdvancedElasticImpact&& ) = default;
    ~AdvancedElasticImpact() = default;
private:
    void BuildMap();
    double FindMaxRadius() const;
private:
    std::vector<ClassicEntity>& m_Entitys;
    std::unordered_map<CellKoords, std::vector<const ClassicEntity*> , CellHash> m_Cells;
    uint32_t m_CellSize;

    std::vector<const ClassicEntity*> m_EntityStash;
    std::vector<std::vector<const ClassicEntity*>> m_Components;

}; 


}
