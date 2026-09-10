#include "Impact.h"

#include "Vector.h"


namespace Physik 
{
void ImpactApplier::ApplyImpacts( EntityRegistry& state, const std::vector<CollisionManifold>& CollisionGroup )
{
    for( auto& Collision : CollisionGroup )
    {
        ClassicEntity& ent1 = state.getById(Collision.ent1);
        ClassicEntity& ent2 = state.getById(Collision.ent2);

        Vec3D vAfter1 = CalcVAfter(ent1, ent2, Collision.normal);
        Vec3D vAfter2 = CalcVAfter(ent2, ent1, Collision.normal);

        Vec3D posAfter1 = CalcPositionCorrection(ent1, ent2, Collision.normal, Collision.Penetration, 1.0);
        Vec3D posAfter2 = CalcPositionCorrection(ent2, ent1, Collision.normal, Collision.Penetration, -1.0);

        ent1.setVelocity(vAfter1);
        ent2.setVelocity(vAfter2);

        ent1.setPosition(posAfter1);
        ent2.setPosition(posAfter2);
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
