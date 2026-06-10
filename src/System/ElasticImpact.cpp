#include "Entity.h"
#include "Impact.h"
#include "Vector.h"
#include <cmath>
#include <cstddef>

namespace Physik 
{
void ElasticImpact::ApplyImpacts( SimulationState& state ) 
{
    for( size_t i = 0; i < state.size(); ++i )
    {
        ClassicEntity& ent1 = state[i];
        for( size_t j = i; j < state.size(); ++j )
        {
            ClassicEntity& ent2 = state[j];
            Vec3D diff = ent2.getPosition() - ent1.getPosition();
            if( diff.EukNorm() <= ent1.getRadius() + ent2.getRadius() )
            {
                //Apply IMpact
            }
        }
    }
}
}
