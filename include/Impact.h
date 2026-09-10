#pragma once

#include "Entity.h"
#include "EntityRegistry.h"
#include "Vector.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Physik
{
struct CollisionManifold 
{ 
    EntityRegistry::ID ent1, ent2; 
    double Penetration;
    Vec3D normal;
    Vec3D contactPoint;

    bool operator==(const CollisionManifold& other) const { return ent1 == other.ent1 && ent2 == other.ent2; }
};

struct CollisionPairHash
{
    std::size_t operator() (const CollisionManifold& pair) const 
    {
        std::size_t h1 = std::hash<uint64_t>{}(pair.ent1);
        std::size_t h2 = std::hash<uint64_t>{}(pair.ent2);

        return h1 ^ (h2 << 1);
    }
};


class ImpactVelocityOperattions 
{
public:
    Vec3D CalcVelocityNormal( const Vec3D& veclocity, const Vec3D& normal ) const;
    Vec3D CalcVelocityTangential( const Vec3D& veclocity, const Vec3D& normal ) const;
    double CalcVelocityAfter( const ClassicEntity& target, const Vec3D& targetNormal, const ClassicEntity& other, const Vec3D& otherNormal ) const;
};

class ImpactApplier 
{
public:
    void ApplyImpacts( EntityRegistry& State, const std::vector<CollisionManifold>& EntityIdx );
public:
    ImpactApplier() = default;
    ImpactApplier(const ImpactApplier&) = default;
    ImpactApplier(ImpactApplier&&) = default;
    ~ImpactApplier() = default;
private:
    Vec3D CalcVAfter(  const ClassicEntity& hited, const ClassicEntity& hitee, Vec3D VectorNormal ) const;
    Vec3D CalcPositionCorrection( const ClassicEntity& hited, const ClassicEntity& hitee, Vec3D VectorNormal, double Penetration, double sign ) const;
private:
    ImpactVelocityOperattions ops;
};


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
    static const std::array<CellKoords, 14> offsets;
public:
    void BuildMap( const EntityRegistry& Entitys );
    void FindAllKollisionPairs( const EntityRegistry& Entitys );
    std::vector<std::vector<CollisionManifold>> getZusammenhangskomponenten( const EntityRegistry& Entitys ) const;

    double getCellSize() const { return m_CellSize; }
public:
    SpartialHashGrid(double CellSize) : m_CellSize(CellSize){}
    SpartialHashGrid( const SpartialHashGrid&) = default;
    SpartialHashGrid( SpartialHashGrid&& ) = default;
    ~SpartialHashGrid() = default;
private:
    void CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2, const EntityRegistry& entitys );
private:
    std::unordered_map<CellKoords, std::vector<EntityRegistry::ID> , CellHash> m_Cells;
    std::unordered_set<CollisionManifold, CollisionPairHash> m_KollisionPairs;

    double m_CellSize;
};

class IImpactEvaluator  
{
public:
    virtual ~IImpactEvaluator() = default;
    virtual void ApplyImpacts( EntityRegistry& state ) = 0;
    virtual std::unique_ptr<IImpactEvaluator> clone() const = 0;
};

class AdvancedElasticImpact : public IImpactEvaluator 
{
public:
    void ApplyImpacts( EntityRegistry& state ) override;
    std::unique_ptr<IImpactEvaluator> clone() const  override { return std::make_unique<AdvancedElasticImpact>(m_Grid.getCellSize()); }
public:
    AdvancedElasticImpact(double CellSize) : m_Grid(CellSize) {}
    AdvancedElasticImpact( const AdvancedElasticImpact& other ) = default;
    AdvancedElasticImpact( AdvancedElasticImpact&& ) = default;
    ~AdvancedElasticImpact() = default;
private:
    SpartialHashGrid m_Grid; //find Impacts
    ImpactApplier m_ImpactApplier; //Apply Impacts
}; 
}
