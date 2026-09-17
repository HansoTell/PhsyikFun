#include "AABB.h"
#include "Entity.h"
#include "Vector.h"
#include <cstddef>
#include <cstdlib>


namespace Physik 
{
AABB getAABB( const Sphere<>& sphere, const Vec3D& position, const Quaternion<>& Rotation ) { return { position - Vec3D{sphere.m_Radius, sphere.m_Radius, sphere.m_Radius}, position + Vec3D{sphere.m_Radius, sphere.m_Radius, sphere.m_Radius } };}

struct RotationMatrix { Vec3D u,v,w; };

RotationMatrix getRotationMatrix( const Quaternion<>& RotationQuaternion )
{
    double x0 = RotationQuaternion[0];
    double x1 = RotationQuaternion[1];
    double x2 = RotationQuaternion[2];
    double x3 = RotationQuaternion[3];

    RotationMatrix erg;

    erg.u = { 1.0 - 2.0 * (x2*x2 + x3*x3), 2.0*x0*x3+2*x1*x2, -2*x0*x2+2.0*x1*x3 };
    erg.v = { -2.0*x0*x3+2.0*x1*x2, 1.0 - 2.0*(x1*x1+x3*x3), 2.0*x0*x1+2.0*x2*x3 };
    erg.w = { 2.0*x0*x2+2.0*x1*x3, -2.0*x0*x1+2.0*x2*x3, 1.0 - 2.0*(x1*x1 + x2*x2) };

    return erg;
}

AABB getAABB( const Box<>& Box, const Vec3D& position, const Quaternion<>& Rotation )  
{ 
    RotationMatrix rot = getRotationMatrix(Rotation);

    Vec3D a;
    for( size_t i = 0; i < a.size(); ++i )
        a[i] = Box.halfSize[1] * std::abs(rot.u[i]) + Box.halfSize[2] * std::abs(rot.v[i]) + Box.halfSize[3] * std::abs(rot.w[i]); 

    return { position - a, position + a };
}
}
