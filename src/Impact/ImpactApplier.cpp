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

        auto Impulse = CalcImpulse(ent1, ent2, Collision);

        if(!Impulse.has_value()) continue;

        Vec3D positionCorrection = CalcPositionCorrection(ent1, ent2, Collision);

        ent1.setVelocity(ent1.getVelocity() - ent1.getInverseMass() * (*Impulse));
        ent2.setVelocity(ent2.getVelocity() + ent2.getInverseMass() * (*Impulse));

        ent1.setPosition(ent1.getPosition() - positionCorrection * ent1.getInverseMass());
        ent2.setPosition(ent2.getPosition() - positionCorrection * ent2.getInverseMass());
    }
}
  
Vec3D ImpactApplier::CalcPositionCorrection( const ClassicEntity& hited, const ClassicEntity& hitee, const CollisionManifold& collision ) const
{
    return (collision.normal * collision.Penetration) / (hited.getInverseMass() + hitee.getInverseMass());
}

std::optional<Vec3D> ImpactApplier::CalcImpulse(  const ClassicEntity& ent1, const ClassicEntity& ent2, const CollisionManifold& collision ) const
{
    using Math::VectorCalc::VectorProduct;
    Vec3D relativVelocity = ent2.getVelocity() - ent1.getVelocity();
    double relativVelocNormal = VectorProduct(relativVelocity, collision.normal);
    if( relativVelocNormal < 0 ) return std::nullopt;

    double inverseMassSum = ent1.getInverseMass() + ent2.getInverseMass();
    auto CombinedMaterial = m_Combiner->CombineMaterials(ent1.getMaterial(), ent2.getMaterial());

    double AbsImpulse = -((1.0 + CombinedMaterial.Restitution) * relativVelocNormal) / inverseMassSum;

    return AbsImpulse * collision.normal;
}
}
