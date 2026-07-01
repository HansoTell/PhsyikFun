#include "Impact.h"

#include "Math/Math.h"


namespace Physik 
{

//Bessere anpassung an drift weg
void ImpactApplier::ApplyImpacts( EntityRegistry& state, const std::vector<size_t>& EntitysIdx )
{
    for( size_t i = 0; i < EntitysIdx.size(); ++i )
    {
        ClassicEntity& ent1 = state[i];
        for( size_t j = i; j < EntitysIdx.size(); ++j )
        {
            ClassicEntity& ent2 = state[j];
            Vec3D diff = ent2.getPosition() - ent1.getPosition();
            if( diff.EukNorm() <= ent1.getRadius() + ent2.getRadius() )
            {
                using namespace Math::VectorCalc;
                Vec3D normal = VectorNormal(ent1.getPosition(), ent2.getPosition());

                Vec3D v1n = ops.CalcVeclocityNormal(ent1.getVelocity(), normal);
                Vec3D v2n = ops.CalcVeclocityNormal(ent2.getVelocity(), normal);

                Vec3D v1t = ops.CalcVelocityTangential(ent1.getVelocity(), normal);
                Vec3D v2t = ops.CalcVelocityTangential(ent2.getVelocity(), normal);

                double vAfter1n = ops.CalcVelocAfter(ent1, v1n, ent2, v2n);
                double vAfter2n = ops.CalcVelocAfter(ent2, v2n, ent1, v1n);

                Vec3D vAfter1 = vAfter1n * v1n + v1t;
                Vec3D vAfter2 = vAfter2n * v2n + v2t;

                ent1.setVelocity(vAfter1);
                ent2.setVelocity(vAfter2);
            }
        }
    }
}
}
