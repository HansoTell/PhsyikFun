#include "Entity.h"
#include "EntityRegistry.h"
#include "Impact.h"
#include "Datastructures/DisjointSets.h"
#include <array>
#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

namespace Physik 
{
AdvancedElasticImpact::AdvancedElasticImpact( EntityRegistry& entitys ) : m_Entitys(entitys) 
{ 
    m_Kollision.reserve(entitys.size() * 2);
}

const std::array<AdvancedElasticImpact::CellKoords, 25> AdvancedElasticImpact::offsets = 
{
    AdvancedElasticImpact::CellKoords{ -1, 1, 0 },
    AdvancedElasticImpact::CellKoords{ 0, 1, 0 },
    AdvancedElasticImpact::CellKoords{ 1, 1, 0 },

    AdvancedElasticImpact::CellKoords{ -1, 0, 0 },
    AdvancedElasticImpact::CellKoords{ 0, 0, 0 }, 
    AdvancedElasticImpact::CellKoords{ 1, 0, 0 },

    AdvancedElasticImpact::CellKoords{ -1, -1, -1 },
    AdvancedElasticImpact::CellKoords{ 0, -1, 1 },
    AdvancedElasticImpact::CellKoords{ 1, -1, 1 },

    AdvancedElasticImpact::CellKoords{ -1, 0, 1 },
    AdvancedElasticImpact::CellKoords{ 0, 0, 1 },
    AdvancedElasticImpact::CellKoords{ 1, 0, 1 },

    AdvancedElasticImpact::CellKoords{ -1, 1, 1 },
    AdvancedElasticImpact::CellKoords{ 0, 1, 1 },
    AdvancedElasticImpact::CellKoords{ 1, 1, 1 }
};

void AdvancedElasticImpact::CollectCollisions( const std::vector<EntityRegistry::ID>& cell1, const std::vector<EntityRegistry::ID>& cell2 )
{
    for( size_t i = 0; i < cell1.size(); ++i )
    {
        auto& ent1ID = cell1[i];
        for( size_t j = 0; j < cell2.size(); ++j )
        {
            auto& ent2ID = cell2[j];
            if( ent1ID == ent2ID )
                continue;

            auto& ent1 = m_Entitys.getById(ent1ID);
            auto& ent2 = m_Entitys.getById(ent2ID);
            
            Vec3D diff = ent2.getPosition() - ent1.getPosition();
            if( diff.EukNorm() > ent1.getRadius() + ent2.getRadius() )
                continue;

            m_Kollision.push_back( { ent1ID, ent2ID } );
        }
    }
}

void AdvancedElasticImpact::ApplyImpacts( SimulationState& state )
{
    using  CellKoords = AdvancedElasticImpact::CellKoords;
    m_CellSize = 2.0 * FindMaxRadius() + 1.0;

    BuildMap();
    
    //find all Kollision pairs
    for( auto&[Cell, Entitys] : m_Cells )
    {
        auto& cellEntitys = m_Cells[Cell];
        for( const auto& offset : AdvancedElasticImpact::offsets)
        {
            CellKoords neighbour { Cell.x_Koord + offset.x_Koord, Cell.y_Koord+offset.y_Koord, Cell.z_Koord+offset.z_Koord };

            auto& neighbourEntitys = m_Cells[neighbour];
            CollectCollisions(cellEntitys, neighbourEntitys);
        }
    }

    //find Komponents with Union datastrucutre
    ds::DisjointSets Union(m_Entitys.size());
    for( auto&[ent1ID, ent2ID] : m_Kollision )
    {
        size_t ent1Idx = m_Entitys.getEntityIdx(ent1ID);
        size_t ent2Idx = m_Entitys.getEntityIdx(ent2ID);

        Union.unite(ent1Idx, ent2Idx);
    }
    m_Kollision.clear();


    //In der disjoint sets iwie methode bauen die die einzelnen sets zurück gibt
    //impacts auflösen
    //Bessere anpassung an drift weg
}

double AdvancedElasticImpact::FindMaxRadius() const
{
    double MaxRadius = std::numeric_limits<double>::min();
    for( size_t i = 0; i < m_Entitys.size(); ++i )
    {
        auto rad = m_Entitys[i].getRadius();
        if( rad > MaxRadius) MaxRadius = rad;
    }

    return MaxRadius;
}

void AdvancedElasticImpact::BuildMap()
{
    size_t avrg_bucketsize = m_Entitys.size()/4;

    m_Cells.clear();
    for( const auto& ent : m_Entitys )
    {
        auto CellKey = CellKoords::getCell(ent.getPosition());

        if(m_Cells.find(CellKey) == m_Cells.end())
            m_Cells[CellKey].reserve(avrg_bucketsize);

        m_Cells[CellKey].push_back(ent.getID());
    }
}
    
}
