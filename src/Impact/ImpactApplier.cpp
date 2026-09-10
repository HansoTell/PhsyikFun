#include "Impact.h"

#include "Vector.h"
#include "Math/Math.h"
#include <cassert>
#include <optional>


namespace Physik 
{
void ImpactApplier::ApplyImpacts( EntityRegistry& state, const std::vector<CollisionManifold>& CollisionGroup )
{
    for( auto& Collision : CollisionGroup )
    {
        ClassicEntity& ent1 = state.getById(Collision.ent1);
        ClassicEntity& ent2 = state.getById(Collision.ent2);

        auto vAfter1 = CalcImpulse(ent1, ent2, Collision);
        auto vAfter2 = CalcImpulse(ent2, ent1, Collision);
        assert((vAfter1.has_value() && vAfter2.has_value()) || (!vAfter1.has_value() && !vAfter2.has_value()));
        if(!vAfter1.has_value()) continue;

        Vec3D posAfter1 = CalcPositionCorrection(ent1, ent2, Collision);
        Vec3D posAfter2 = CalcPositionCorrection(ent2, ent1, Collision);

        ent1.setVelocity(vAfter1.value());
        ent2.setVelocity(vAfter2.value());

        ent1.setPosition(posAfter1);
        ent2.setPosition(posAfter2);
    }
}
  
Vec3D ImpactApplier::CalcPositionCorrection( const ClassicEntity& hited, const ClassicEntity& hitee, const CollisionManifold& collision ) const
{
    double weightHited = 1/hited.getMass(); 
    double weightHitee = 1/hitee.getMass(); 
    double weightGes = weightHited + weightHitee;

    return hited.getPosition() +sign * VectorNormal * Penetration * weightHited / weightGes;
}

std::optional<Vec3D> ImpactApplier::CalcImpulse(  const ClassicEntity& ent1, const ClassicEntity& ent2, const CollisionManifold& collision ) const
{
    using Math::VectorCalc::VectorProduct;
    Vec3D relativVelocity = ent2.getVelocity() - ent1.getVelocity();
    double relativVelocNormal = VectorProduct(relativVelocity, collision.normal);
    if( relativVelocNormal < 0 ) return std::nullopt;

    double inverseMassSum = ent1.getInverseMass() + ent2.getInverseMass();

    double AbsImpulse = -()



    return std::nullopt;
}
}
