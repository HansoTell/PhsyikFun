#include "AABB.h"
#include "Vector.h"

namespace Physik 
{
AABB getAABB( const Sphere<>& sphere, const Vec3D& position) { return { position - Vec3D{sphere.m_Radius, sphere.m_Radius, sphere.m_Radius}, position + Vec3D{sphere.m_Radius, sphere.m_Radius, sphere.m_Radius } };}
AABB getAABB( const Box<>& Box, const Vec3D& position)  { return { position - Box.halfSize, position + Box.halfSize }; }
}
