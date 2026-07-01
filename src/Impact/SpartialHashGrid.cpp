#include "Impact.h"

#include "Datastructures/DisjointSets.h"


namespace Physik 
{
const std::array<SpartialHashGrid::CellKoords, 25> SpartialHashGrid::offsets = 
{
    SpartialHashGrid::CellKoords{ -1, 1, 0 },
    SpartialHashGrid::CellKoords{ 0, 1, 0 },
    SpartialHashGrid::CellKoords{ 1, 1, 0 },

    SpartialHashGrid::CellKoords{ -1, 0, 0 },
    SpartialHashGrid::CellKoords{ 0, 0, 0 }, 
    SpartialHashGrid::CellKoords{ 1, 0, 0 },

    SpartialHashGrid::CellKoords{ -1, -1, -1 },
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

void SpartialHashGrid::BuildMap( const EntityRegistry& Entitys)
{
    size_t avrg_bucketsize = Entitys.size()/4;
    auto maxRadius = std::max_element(Entitys.begin(), Entitys.end(), [](const ClassicEntity& ent1, const ClassicEntity& ent2){
        return ent1.getRadius() < ent2.getRadius();
    });   

    double CellSize = 2 * maxRadius->getRadius() + 1; 
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

            auto& neighbourEntitys = m_Cells.at(neighbour);
            CollectCollisions(cellEntitys, neighbourEntitys, Entitys);
        }
    }
}

void SpartialHashGrid::CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2, const EntityRegistry& Entitys ) 
{
    for( size_t i = 0; i < cell1.size(); ++i )
    {
        auto& ent1ID = cell1[i];
        for( size_t j = 0; j < cell2.size(); ++j )
        {
            auto& ent2ID = cell2[j];
            if( ent1ID == ent2ID )
                continue;

            auto& ent1 = Entitys.getById(ent1ID);
            auto& ent2 = Entitys.getById(ent2ID);
            
            Vec3D diff = ent2.getPosition() - ent1.getPosition();
            if( diff.EukNorm() > ent1.getRadius() + ent2.getRadius() )
                continue;

            m_Kollision.push_back( { ent1ID, ent2ID } );
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
