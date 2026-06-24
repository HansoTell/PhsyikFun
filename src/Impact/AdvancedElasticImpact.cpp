#include "Impact.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace Physik 
{
AdvancedElasticImpact::AdvancedElasticImpact( std::vector<ClassicEntity>& entitys ) : m_Entitys(entitys) {}

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

void AdvancedElasticImpact::ApplyImpacts( SimulationState& state )
{
    using  CellKoords = AdvancedElasticImpact::CellKoords;
    m_CellSize = 2.0 * FindMaxRadius() + 1.0;

    BuildMap();
    //Welche datdasrtuctres für den Tree? -> I guess vector von vectoren? --> Wie gut reserven?
    
    for( auto&[Cell, Entitys] : m_Cells )
    {
        for( const auto& offset : AdvancedElasticImpact::offsets)
        {
            CellKoords neighbour { Cell.x_Koord + offset.x_Koord, Cell.y_Koord+offset.y_Koord, Cell.z_Koord+offset.z_Koord };

            // Jetzt diese Gruppen bilden von Kollisionen i.g -> Cell lookuppen und dann alle abholen

        }
    }


    //Finden wir diesen impact tree
    //Bessere anpassung an drift weg
    //
    //Kollisionsgraph --> Alle Gruppen von Kollisionen finden
    
    for( uint8_t i = 0; i < 10; ++i )
    {
        //Impact actually applayen
        //aufpassen dass keine Doppel Zellen überprüfung
    }

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
    m_Cells.clear();
    for( const auto& ent : m_Entitys )
    {
        auto CellKey = CellKoords::getCell(ent.getPosition());

        m_Cells[CellKey].push_back(&ent);
    }
}
    
}
