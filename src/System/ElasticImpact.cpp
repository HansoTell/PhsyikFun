#include "Entity.h"
#include "Impact.h"
#include "Math/Math.h"
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
                using namespace Math::VectorCalc;
                Vec3D normal = VectorNormal(ent1.getPosition(), ent2.getPosition());

                Vec3D v1n = CalcVeclocityNormal(ent1.getVelocity(), normal);
                Vec3D v2n = CalcVeclocityNormal(ent2.getVelocity(), normal);

                Vec3D v1t = CalcVelocityTangential(ent1.getVelocity(), normal);
                Vec3D v2t = CalcVelocityTangential(ent2.getVelocity(), normal);

                double vAfter1n = CalcVelocAfter(ent1, v1n, ent2, v2n);
                double vAfter2n = CalcVelocAfter(ent2, v2n, ent1, v1n);

                Vec3D vAfter1 = vAfter1n * v1n + v1t;
                Vec3D vAfter2 = vAfter2n * v2n + v2t;

                ent1.setVelocity(vAfter1);
                ent2.setVelocity(vAfter2);
            }
        }
    }
}

Vec3D ElasticImpact::CalcVeclocityNormal( const Vec3D& veclocity, const Vec3D& normal ) const
{
    using Math::VectorCalc::VectorProduct;
    return (VectorProduct(veclocity, normal)) * normal;
}

Vec3D ElasticImpact::CalcVelocityTangential( const Vec3D& veclocity, const Vec3D& normal ) const
{
    using Math::VectorCalc::VectorProduct;
    return veclocity - (VectorProduct(veclocity, normal)*normal);

}

double ElasticImpact::CalcVelocAfter( const ClassicEntity& target, const Vec3D& targetNormal, const ClassicEntity& other, const Vec3D& otherNormal ) const
{
    double targetmass = target.getMass();
    double otherMass = other.getMass();
    double MassGes = targetmass+otherMass;                                                                                                
    
    return (targetmass - otherMass) * targetNormal.EukNorm() / MassGes + 2 * otherMass * otherNormal.EukNorm() / MassGes;
}
}
