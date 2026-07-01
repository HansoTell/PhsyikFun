#include "EntityRegistry.h"
#include "Impact.h"
#include <array>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Physik 
{
AdvancedElasticImpact::AdvancedElasticImpact() : m_Grid() {}

//TODO
AdvancedElasticImpact::AdvancedElasticImpact( const AdvancedElasticImpact& other ) : m_Grid(other.m_Grid){}

void AdvancedElasticImpact::ApplyImpacts( EntityRegistry& state )
{
    m_Grid.BuildMap( state );
    
    m_Grid.FindAllKollisionPairs( state );

    std::unordered_map<size_t, std::vector<size_t>> groups = m_Grid.getZusammenhangskomponenten( state );

    //resolveImpacts
    for( auto&[rep, set] : groups )
    {
        switch (set.size()) 
        {
        case 1:
            break;
        case 2:
        {
            m_ImpactApplier.ApplyImpacts(state, set);
            break;
        }
        default:
        {
            for( int i = 0; i < 10; ++i )
                m_ImpactApplier.ApplyImpacts(state, set);
            break;
        }
        }

    }
}
}
