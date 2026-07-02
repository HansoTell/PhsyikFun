#include "Impact.h"

#include "Math/Math.h"


namespace Physik 
{
Vec3D ImpactVelocityOperattions::CalcVelocityNormal( const Vec3D& veclocity, const Vec3D& normal ) const
{
    using Math::VectorCalc::VectorProduct;
    return (VectorProduct(veclocity, normal)) * normal;
}
Vec3D ImpactVelocityOperattions::CalcVelocityTangential( const Vec3D& veclocity, const Vec3D& normal ) const
{
    using Math::VectorCalc::VectorProduct;
    return veclocity - (VectorProduct(veclocity, normal)*normal);
}
double ImpactVelocityOperattions::CalcVelocityAfter( const ClassicEntity& target, const Vec3D& targetNormal, const ClassicEntity& other, const Vec3D& otherNormal ) const
{
    double targetmass = target.getMass();
    double otherMass = other.getMass();
    double MassGes = targetmass+otherMass;                                                                                                
    
    return (targetmass - otherMass) * targetNormal.EukNorm() / MassGes + 2 * otherMass * otherNormal.EukNorm() / MassGes;
}
    
}
