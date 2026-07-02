#include "Entity.h"
#include "EntityRegistry.h"
#include "Impact.h"
#include "Math/Math.h"
#include "Vector.h"
#include <cmath>
#include <cstddef>

namespace Physik 
{
void ElasticImpact::ApplyImpacts( EntityRegistry& state ) 
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
                using namespace Math::VectorCalc;
                Vec3D normal = VectorNormal(ent1.getPosition(), ent2.getPosition());

                Vec3D v1n = ops.CalcVelocityNormal(ent1.getVelocity(), normal);
                Vec3D v2n = ops.CalcVelocityNormal(ent2.getVelocity(), normal);

                Vec3D v1t = ops.CalcVelocityTangential(ent1.getVelocity(), normal);
                Vec3D v2t = ops.CalcVelocityTangential(ent2.getVelocity(), normal);

                double vAfter1n = ops.CalcVelocityAfter(ent1, v1n, ent2, v2n);
                double vAfter2n = ops.CalcVelocityAfter(ent2, v2n, ent1, v1n);

                Vec3D vAfter1 = vAfter1n * v1n + v1t;
                Vec3D vAfter2 = vAfter2n * v2n + v2t;

                ent1.setVelocity(vAfter1);
                ent2.setVelocity(vAfter2);
            }
        }
    }
}
}
