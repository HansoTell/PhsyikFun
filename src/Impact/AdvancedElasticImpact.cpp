#include "EntityRegistry.h"
#include "Impact.h"
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
    auto groups = m_Grid.getZusammenhangskomponenten( state );

    //resolveImpacts
    //O(size of Zusammenhangskomponente)
    for( auto& set : groups )
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
