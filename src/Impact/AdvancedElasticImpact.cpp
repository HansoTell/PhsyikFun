#include "Impact.h"
#include <cstddef>
#include <limits>

namespace Physik 
{
AdvancedElasticImpact::AdvancedElasticImpact( std::vector<ClassicEntity>& entitys ) : m_Entitys(entitys) {}

void AdvancedElasticImpact::ApplyImpacts( SimulationState& state )
{

}

void AdvancedElasticImpact::BuildMap()
{
    m_Map.clear();

    //Find Max Radius -> O(n)
    double MaxRadius = std::numeric_limits<double>::min();
    for( size_t i = 0; i < m_Entitys.size(); ++i )
    {
        auto rad = m_Entitys[i].getRadius();
        if( rad > MaxRadius) MaxRadius = rad;
    }
    m_CellSize = MaxRadius;

    //Als nächstes zuteilen


}
    
}
