#include "EntityRegistry.h"
#include "Impact.h"
#include <array>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Physik 
{
void AdvancedElasticImpact::ApplyImpacts( EntityRegistry& state )
{
    //O(n)
    m_Grid.BuildMap( state );
    
    //O(SizeofBuckets²)
    m_Grid.FindAllKollisionPairs( state );

    //O(alpha * n)
    std::unordered_map<size_t, std::vector<size_t>> groups = m_Grid.getZusammenhangskomponenten( state );

    //resolveImpacts
    //O(size of Zusammenhangskomponente)
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
            constexpr int solver_iterations = 10; //Komplett heuristisch
            for( int i = 0; i < solver_iterations; ++i )
                m_ImpactApplier.ApplyImpacts(state, set);
            break;
        }
        }

    }
}
}
