#include "Impact.h"

#include "Datastructures/DisjointSets.h"
#include "CollisionDetction.h"

#include <cstddef>
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

SpartialHashGrid::SpartialHashGrid() {}

//TODO: Problem wie mache ich die cell größe -> müssen eigentlich mehrere cells ein entity erlauben
void SpartialHashGrid::BuildMap( const EntityRegistry& Entitys)
{
    if( Entitys.empty() )
    {
        m_Cells.clear();
        return;
    }

    m_Cells.clear();
    for( const auto& ent : Entitys )
    {
        auto CellKey = CellKoords::getCell(ent.getPosition(), CellSize);

        if(m_Cells.find(CellKey) == m_Cells.end())
            m_Cells[CellKey].reserve(avrg_bucketsize);

        m_Cells[CellKey].push_back(ent.getID());
    }
}

void SpartialHashGrid::FindAllKollisionPairs( const EntityRegistry& Entitys ) 
{
    m_Kollision.clear();

    for( auto&[Cell, _] : m_Cells )
    {
        auto& cellEntitys = m_Cells.at(Cell);
        for( const auto& offset : SpartialHashGrid::offsets)
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

            auto collision = std::visit([&]( const auto& ShapeA, const auto& ShapeB ) -> std::optional<CollisionPair> 
            { 
                return detectCollision(ShapeA, ShapeB, ent1.getPosition(), ent2.getPosition()); 
            }, ent1.getShape(), ent2.getShape());
            

            if( collision.has_value() ) m_Kollision.push_back(collision.value());
        }
    }
}

std::unordered_map<size_t, std::vector<size_t>> SpartialHashGrid::getZusammenhangskomponenten( const EntityRegistry& Entitys ) const
{
    ds::DisjointSets Union(Entitys.size());
    for( auto&[ent1ID, ent2ID] : m_Kollision)
    {
        size_t ent1Idx = Entitys.getEntityIdx(ent1ID);
        size_t ent2Idx = Entitys.getEntityIdx(ent2ID);

        Union.unite(ent1Idx, ent2Idx);
    }

    std::unordered_map<size_t, std::vector<size_t>> groups = Union.getSets();

    return groups;
}
    
}
