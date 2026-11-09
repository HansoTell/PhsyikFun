#include "Vector.h"
#include "Shapes.h"
#include <Math/Quaternion.h>

namespace Physik 
{

struct AABB
{
    Vec3D min, max;
};

AABB getAABB( const Sphere<>& sphere, const Vec3D& position, const Quaternion<>& Rotation ); 
AABB getAABB( const Box<>& sphere, const Vec3D& position, const Quaternion<>& Rotation ); 
    
}
