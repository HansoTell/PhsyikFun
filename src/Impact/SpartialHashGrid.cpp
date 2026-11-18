#include "EntityRegistry.h"
#include "Impact.h"

#include "CollisionDetction.h"
#include "AABB.h"
#include "Vector.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>


namespace Physik 
{
void SpartialHashGrid::UpdateCellSize( const EntityRegistry& Entitys, const std::vector<EntityRegistry::ID>& EntitySet )
{
    if( m_isCellSizeStatic )
        return; 

    constexpr double Percentile = 0.7;

    if( EntitySet.size() == 0 )
        return;

    std::vector<double> EntityBoundingRadius;
    EntityBoundingRadius.reserve(EntitySet.size());

    for( auto& EntID : EntitySet )
    {
        const auto& ent = Entitys.getById(EntID); 
        auto AABB = std::visit([&](const auto& Shape){
            return getAABB(Shape, ent.getPosition(), ent.getRotation());
        }, ent.getShape());
        EntityBoundingRadius.push_back((AABB.max - ent.getPosition()).EukNorm());
    }
    std::sort(EntityBoundingRadius.begin(), EntityBoundingRadius.end());

    m_CellSize = 2.0 * EntityBoundingRadius[std::floor(Percentile * EntityBoundingRadius.size())];
}

void SpartialHashGrid::BuildMap( const EntityRegistry& Entitys, const std::vector<EntityRegistry::ID>& EntitySet )
{
    m_CellEntry.clear();

    if( EntitySet.empty() )
        return;

    for( const auto& EntityID : EntitySet )
    {
        const auto& ent = Entitys.getById(EntityID);
        auto AABBBox = std::visit([&](const auto& Shape)
        {
            return getAABB(Shape, ent.getPosition(), ent.getRotation());
        }, ent.getShape());

        CellKoords minCell = CellKoords::getCell(AABBBox.min, m_CellSize);
        CellKoords maxCell = CellKoords::getCell(AABBBox.max, m_CellSize);

        for( int32_t x = minCell.x_Koord; x <= maxCell.x_Koord; ++x )
            for( int32_t y = minCell.y_Koord; y <= maxCell.y_Koord; ++y )
                for( int32_t z = minCell.z_Koord; z <= maxCell.z_Koord; ++z )
                    m_CellEntry.push_back({ PackCell({x, y, z}), EntityID } );
    }
    std::sort(m_CellEntry.begin(), m_CellEntry.end(), [&]( const CellEntry& a, const CellEntry& b){
        return a.key != b.key ? a.key < b.key : a.id < b.id;
    });
}

CollisionSet SpartialHashGrid::FindAllKollisionPairs( const EntityRegistry& Entitys ) 
{
    CollisionSet Collisions;
    Collisions.reserve(Entitys.size());

    const size_t N = m_CellEntry.size();
    size_t begin = 0;
    while(begin < N)
    {
        size_t end = begin+1;
        while( end < N && m_CellEntry[end].key == m_CellEntry[begin].key) ++end;

        for( size_t i = begin; i < end; ++i )
        {
            const auto& ent1 = Entitys.getById(m_CellEntry[i].id);
            for( size_t j = i+1; j < end; ++j )
            {
                const auto& ent2 = Entitys.getById(m_CellEntry[j].id);
                auto collision_or = FindCollision(ent1, ent2);
                if( !collision_or.has_value()) continue;
                Collisions.insert(collision_or.value());
            }
        }
        begin = end;
    }

    return Collisions;
}

void SpartialHashGrid::CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2, const EntityRegistry& Entitys, CollisionSet& outCollisions ) 
{
    for( size_t i = 0; i < cell1.size(); ++i )
    {
        auto& ent1ID = cell1[i];
        size_t startIndex = &cell1 == &cell2 ? i+1 : 0;
        for( size_t j = startIndex; j < cell2.size(); ++j )
        {
            auto& ent2ID = cell2[j];
            if( ent1ID == ent2ID )
                continue;

            auto& ent1 = Entitys.getById(ent1ID);
            auto& ent2 = Entitys.getById(ent2ID);

            auto collision_or = FindCollision(ent1, ent2);

            if( !collision_or.has_value() ) continue;
            auto& collision = collision_or.value();
            if(outCollisions.find(collision) != outCollisions.end()) continue;

            outCollisions.insert(collision);
        }
    }
}
}
