#include "Entity.h"
#include "Impact.h"
#include <array>
#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

namespace Physik 
{
AdvancedElasticImpact::AdvancedElasticImpact( std::vector<ClassicEntity>& entitys ) : m_Entitys(entitys) 
{ 
    m_CellEntitysStash.reserve(entitys.size());
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

struct CollisionPair { const ClassicEntity* ent1; const ClassicEntity* ent2; };
static void CollectCollisions( const std::vector<const ClassicEntity*>& entitys, std::vector<CollisionPair>& outPairs )
{
    for( size_t i = 0; i < entitys.size(); ++i )
    {
        for( size_t j = i+1; j < entitys.size(); ++i )
        {
            auto& ent1 = entitys[i];
            auto& ent2 = entitys[j];
            Vec3D diff = ent2->getPosition() - ent1->getPosition();
            if( diff.EukNorm() <= ent1->getRadius() + ent2->getRadius() )
                outPairs.push_back( { ent1, ent2 } );
        }
    }
}

void AdvancedElasticImpact::ApplyImpacts( SimulationState& state )
{
    using  CellKoords = AdvancedElasticImpact::CellKoords;
    m_CellSize = 2.0 * FindMaxRadius() + 1.0;

    BuildMap();
    
    //Hier ist jetzt das Problem nur noch doppelte komponenten... Also wenn bei einem die nicht drinnenn waren das die zusammen gepackt werden
    
    for( auto&[Cell, Entitys] : m_Cells )
    {
        for( const auto& offset : AdvancedElasticImpact::offsets)
        {
            CellKoords neighbour { Cell.x_Koord + offset.x_Koord, Cell.y_Koord+offset.y_Koord, Cell.z_Koord+offset.z_Koord };

            auto& cellEntitys = m_Cells[neighbour];
            for( auto& ent : cellEntitys ) m_CellEntitysStash.push_back(ent);
        }
    }


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

        m_Cells[CellKey].push_back(&ent);
    }
}
    
}
