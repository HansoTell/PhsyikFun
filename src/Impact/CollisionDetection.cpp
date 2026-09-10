#include "CollisionDetction.h"

#include "EntityRegistry.h"
#include "Impact.h"
#include <optional>

namespace Physik 
{
std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Sphere<double>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB) 
{
    Vec3D diff =  posB - posA;

    if( diff.EukNorm() > A.m_Radius + B.m_Radius ) 
        return std::nullopt;

    Vec3D normal = diff/diff.EukNorm();
    double penetration = A.m_Radius + B.m_Radius - diff.EukNorm();

    return CollisionManifold{ idA, idB, penetration, normal };
}


}
