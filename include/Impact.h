#pragma once

#include "Entity.h"
#include "EntityRegistry.h"
#include "Vector.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Physik
{
class ImpactVelocityOperattions 
{
public:
    Vec3D CalcVeclocityNormal( const Vec3D& veclocity, const Vec3D& normal ) const;
    Vec3D CalcVelocityTangential( const Vec3D& veclocity, const Vec3D& normal ) const;
    double CalcVelocAfter( const ClassicEntity& target, const Vec3D& targetNormal, const ClassicEntity& other, const Vec3D& otherNormal ) const;
};

class IImpactEvaluator  
{
public:
    virtual ~IImpactEvaluator() = default;
    virtual void ApplyImpacts( EntityRegistry& state ) = 0;
};

class ElasticImpact : public IImpactEvaluator 
{
public:
    void ApplyImpacts( EntityRegistry& state ) override;
public:
    ElasticImpact() = default;
    ElasticImpact( const ElasticImpact& other ) = default;
    ElasticImpact( ElasticImpact&& other ) = default;
    ~ElasticImpact() = default;
private:
    ImpactVelocityOperattions ops;
};


struct CollisionPair { EntityRegistry::ID ent1, ent2; };

class SpartialHashGrid
{
private:
    struct CellKoords
    {
        int32_t x_Koord, y_Koord, z_Koord;
        bool operator == ( const CellKoords& other ) const { return  this->x_Koord == other.x_Koord && this->y_Koord == other.y_Koord && this->z_Koord == other.z_Koord; }
        static CellKoords getCell( const Vec3D& pos, double cellSize ) 
        { 
            return { 
                static_cast<int32_t>(std::floor(pos[0]/cellSize)), 
                static_cast<int32_t>(std::floor(pos[1]/cellSize)), 
                static_cast<int32_t>(std::floor(pos[2]/cellSize)) 
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
    void BuildMap( const EntityRegistry& Entitys );
    void FindAllKollisionPairs( const EntityRegistry& Entitys );
    std::unordered_map<size_t, std::vector<size_t>> getZusammenhangskomponenten( const EntityRegistry& Entitys ) const;
public:
    SpartialHashGrid();
    SpartialHashGrid( const SpartialHashGrid&) = default;
    SpartialHashGrid( SpartialHashGrid&& ) = default;
    ~SpartialHashGrid() = default;
private:
    void CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2, const EntityRegistry& entitys );
private:
    std::unordered_map<CellKoords, std::vector<EntityRegistry::ID> , CellHash> m_Cells;
    std::vector<CollisionPair> m_Kollision;
};

class ImpactApplier
{
public:
    void ApplyImpacts( EntityRegistry& State, const std::vector<size_t>& EntityIdx );
private:
    ImpactVelocityOperattions ops;
};


class AdvancedElasticImpact : public IImpactEvaluator 
{
public:
    void ApplyImpacts( EntityRegistry& state );
public:
    AdvancedElasticImpact(); 
    AdvancedElasticImpact( const AdvancedElasticImpact& other );
    AdvancedElasticImpact( AdvancedElasticImpact&& ) = default;
    ~AdvancedElasticImpact() = default;
private:
    SpartialHashGrid m_Grid;
    ImpactApplier m_ImpactApplier;
}; 


}
