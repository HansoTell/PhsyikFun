#include "Vector.h"
#include "Shapes.h"

namespace Physik 
{

struct AABB
{
    Vec3D min, max;
};

AABB getAABB( const Sphere<>& sphere, const Vec3D& position); 
AABB getAABB( const Box<>& sphere, const Vec3D& position); 
    
}
