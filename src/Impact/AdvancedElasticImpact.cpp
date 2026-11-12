#include "EntityRegistry.h"
#include "Impact.h"

namespace Physik 
{
void AdvancedElasticImpact::ApplyImpacts( EntityRegistry& state )
{
    //O(n)
    m_Grid.BuildMap( state, state.getDynamicEntitys() );
    
    //O(SizeofBuckets²)
    CollisionSet DynamicCollisions = m_Grid.FindAllKollisionPairs( state );
    CollisionSet StaticCollisions = m_BVH.CollectCollisions(state,  state.getDynamicEntitys());

    DynamicCollisions.reserve(DynamicCollisions.size()+StaticCollisions.size());
    DynamicCollisions.merge(StaticCollisions);

    //O(alpha * n)
    auto groups = m_ConnectedParts.getConnectedParts( state, DynamicCollisions );

    //resolveImpacts
    //O(size of Zusammenhangskomponente)
    for( auto& set : groups )
    {
        switch (set.size()) 
        {
        case 0:
            break;
        case 1:
            m_ImpactApplier.ApplyImpacts(state, set);
            break;
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

void AdvancedElasticImpact::NotifyEntityAddition( const EntityRegistry& state ) 
{ 
    m_Grid.UpdateCellSize( state, state.getDynamicEntitys() ); 
    m_BVH.BuildBVH(state, state.getStaticEntitys());
}
}
