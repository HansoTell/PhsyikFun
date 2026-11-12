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
const std::array<SpartialHashGrid::CellKoords, 14> SpartialHashGrid::offsets = 
{
    SpartialHashGrid::CellKoords{ -1, 1, 0 },
    SpartialHashGrid::CellKoords{ 0, 1, 0 }, 
    SpartialHashGrid::CellKoords{ 1, 1, 0 }, 

    SpartialHashGrid::CellKoords{ 0, 0, 0 }, 
    SpartialHashGrid::CellKoords{ 1, 0, 0 },

    SpartialHashGrid::CellKoords{ -1, -1, 1 },
    SpartialHashGrid::CellKoords{ 0, -1, 1 },
    SpartialHashGrid::CellKoords{ 1, -1, 1 },

    SpartialHashGrid::CellKoords{ -1, 0, 1 }, 
    SpartialHashGrid::CellKoords{ 0, 0, 1 },
    SpartialHashGrid::CellKoords{ 1, 0, 1 },

    SpartialHashGrid::CellKoords{ -1, 1, 1 },
    SpartialHashGrid::CellKoords{ 0, 1, 1 },
    SpartialHashGrid::CellKoords{ 1, 1, 1 }
};

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
    m_Cells.clear();

    if( EntitySet.empty() )
        return;

    size_t avrg_bucketsize = EntitySet.size()/4;
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
        {
            for( int32_t y = minCell.y_Koord; y <= maxCell.y_Koord; ++y )
            {
                for( int32_t z = minCell.z_Koord; z <= maxCell.z_Koord; ++z )
                {
                    CellKoords momCell = { x, y, z };
                    if( m_Cells.find(momCell) == m_Cells.end()) m_Cells[momCell].reserve(avrg_bucketsize);
                    m_Cells[momCell].push_back(EntityID);
                }
            }
        }
    }
}

CollisionSet SpartialHashGrid::FindAllKollisionPairs( const EntityRegistry& Entitys ) 
{
    CollisionSet Collisions;
    Collisions.reserve(Entitys.size());
    for( auto&[Cell, _] : m_Cells )
    {
        auto& cellEntitys = m_Cells.at(Cell);
        for( const auto& offset : SpartialHashGrid::offsets )
        {
            CellKoords neighbour { Cell.x_Koord + offset.x_Koord, Cell.y_Koord+offset.y_Koord, Cell.z_Koord+offset.z_Koord };

            auto neighbour_it = m_Cells.find(neighbour);
            if( neighbour_it == m_Cells.end() ) continue;

            CollectCollisions(cellEntitys, neighbour_it->second, Entitys, Collisions);
        }
    }
    return Collisions;
}

void SpartialHashGrid::CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2, const EntityRegistry& Entitys, CollisionSet& outCollisions ) 
{
    for( size_t i = 0; i < cell1.size(); ++i )
    {
        auto& ent1ID = cell1[i];
        size_t startIndex = cell1 == cell2 ? i+1 : 0;
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
