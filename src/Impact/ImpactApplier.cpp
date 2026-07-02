#include "Impact.h"

#include "Math/Math.h"
#include "Vector.h"


namespace Physik 
{
void ImpactApplier::ApplyImpacts( EntityRegistry& state, const std::vector<size_t>& EntitysIdx )
{
    for( size_t i = 0; i < EntitysIdx.size(); ++i )
    {
        ClassicEntity& ent1 = state[EntitysIdx[i]];
        for( size_t j = i+1; j < EntitysIdx.size(); ++j )
        {
            ClassicEntity& ent2 = state[EntitysIdx[j]];
            if( Vec3D diff = ent2.getPosition() - ent1.getPosition(); diff.EukNorm() <= ent1.getRadius() + ent2.getRadius() )
            {
                using namespace Math::VectorCalc;
                Vec3D normal = VectorNormal(ent1.getPosition(), ent2.getPosition());
                double penetration = ent1.getRadius() + ent2.getRadius() - diff.EukNorm();

                Vec3D vAfter1 = CalcVAfter(ent1, ent2, normal);
                Vec3D vAfter2 = CalcVAfter(ent2, ent1, normal);

                Vec3D posAfter1 = CalcPositionCorrection(ent1, ent2, normal, penetration, 1.0);
                Vec3D posAfter2 = CalcPositionCorrection(ent2, ent1, normal, penetration, -1.0);

                ent1.setVelocity(vAfter1);
                ent2.setVelocity(vAfter2);

                ent1.setPosition(posAfter1);
                ent2.setPosition(posAfter2);
            }
        }
    }
}
  
Vec3D ImpactApplier::CalcPositionCorrection( const ClassicEntity& hited, const ClassicEntity& hitee, Vec3D VectorNormal, double Penetration, double sign ) const
{
    double weightHited = 1/hited.getMass(); 
    double weightHitee = 1/hitee.getMass(); 
    double weightGes = weightHited + weightHitee;

    return hited.getPosition() +sign * VectorNormal * Penetration * weightHited / weightGes;
}

Vec3D ImpactApplier::CalcVAfter(  const ClassicEntity& hited, const ClassicEntity& hitee, Vec3D VectorNormal ) const
{
    Vec3D velocityNormalHited = ops.CalcVelocityNormal(hited.getVelocity(), VectorNormal);
    Vec3D velocityNormalHitee = ops.CalcVelocityNormal(hitee.getVelocity(), VectorNormal);
    Vec3D velocityTangentail = ops.CalcVelocityTangential(hited.getVelocity(), VectorNormal);

    double velocityAfterNormal = ops.CalcVelocityAfter(hited, velocityNormalHited, hitee, velocityNormalHitee);

    Vec3D velocityAfter = velocityAfterNormal * VectorNormal + velocityTangentail;

    return velocityAfter;
   
}
}
