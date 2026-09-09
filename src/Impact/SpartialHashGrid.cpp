#include "Impact.h"

#include "Datastructures/DisjointSets.h"
#include "CollisionDetction.h"
#include "AABB.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>


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

void SpartialHashGrid::BuildMap( const EntityRegistry& Entitys)
{
    if( Entitys.empty() )
    {
        m_Cells.clear();
        return;
    }

    m_Cells.clear();
    size_t avrg_bucketsize = Entitys.size()/4;
    for( const auto& ent : Entitys )
    {
        auto AABBBox = std::visit([&](const auto& Shape)
        {
            return getAABB(Shape, ent.getPosition());
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
                    m_Cells[momCell].push_back(ent.getID());
                }
            }
        }
    }
}

void SpartialHashGrid::FindAllKollisionPairs( const EntityRegistry& Entitys ) 
{
    m_KollisionPairs.clear();

    for( auto&[Cell, _] : m_Cells )
    {
        auto& cellEntitys = m_Cells.at(Cell);
        for( const auto& offset : SpartialHashGrid::offsets )
        {
            CellKoords neighbour { Cell.x_Koord + offset.x_Koord, Cell.y_Koord+offset.y_Koord, Cell.z_Koord+offset.z_Koord };

            auto neighbour_it = m_Cells.find(neighbour);
            if( neighbour_it == m_Cells.end() ) continue;

            CollectCollisions(cellEntitys, neighbour_it->second, Entitys);
        }
    }
}

void SpartialHashGrid::CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2, const EntityRegistry& Entitys ) 
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

            auto collision_or = std::visit([&]( const auto& ShapeA, const auto& ShapeB ) -> std::optional<CollisionPair> 
            { 
                return detectCollision(ShapeA, ShapeB, ent1.getPosition(), ent2.getPosition()); 
            }, ent1.getShape(), ent2.getShape());
            

            if( !collision_or.has_value() ) continue;

            auto& collision = collision_or.value();
            if(collision.ent1 > collision.ent2 ) std::swap(collision.ent1, collision.ent2);
            if(m_KollisionPairs.find(collision) != m_KollisionPairs.end()) continue;

            m_KollisionPairs.insert(collision);
        }
    }
}

std::unordered_map<size_t, std::vector<size_t>> SpartialHashGrid::getZusammenhangskomponenten( const EntityRegistry& Entitys ) const
{
    ds::DisjointSets Union(Entitys.size());
    for( auto& pair : m_KollisionPairs)
    {
        size_t ent1Idx = Entitys.getEntityIdx(pair.ent1);
        size_t ent2Idx = Entitys.getEntityIdx(pair.ent2);

        Union.unite(ent1Idx, ent2Idx);
    }

    std::unordered_map<size_t, std::vector<size_t>> groups = Union.getSets();

    return groups;
}
    
}
