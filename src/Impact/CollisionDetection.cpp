#include "CollisionDetction.h"

#include "EntityRegistry.h"
#include "Impact.h"
#include "Vector.h"
#include <algorithm>
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

static Vec3D ProjectKoordOnBox( const Vec3D& BoxMin, const Vec3D& BoxMax, const Vec3D& posSphere )
{
    return Vec3D{ std::max(BoxMin.at(0), std::min(posSphere.at(0), BoxMax.at(0))), std::max(BoxMin.at(1), std::min(posSphere.at(1), BoxMax.at(1))), std::max(BoxMin.at(2), std::min(posSphere.at(2), BoxMax.at(2))) };
}

std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Box<>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB) 
{
    Vec3D BoxMin = posB - B.halfSize;
    Vec3D BoxMAx = posB + B.halfSize;

    Vec3D nearestPoint = ProjectKoordOnBox(BoxMin, BoxMAx, posA);
    Vec3D BoxSphereDistance = nearestPoint - posA;
    double distance = BoxSphereDistance.EukNorm();
    
    if( A.m_Radius < distance )
        return std::nullopt;

    Vec3D Normal = BoxSphereDistance/distance;
    double Penetrarion = A.m_Radius - distance; 

    return CollisionManifold{ idA, idB, Penetrarion, Normal };
}

std::optional<CollisionManifold> detectCollision(const Box<>& A, const Sphere<>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB) 
{
    return detectCollision(B, A, posB, posA, idB, idA);
}

//TODO:
std::optional<CollisionManifold> detectCollision(const Box<> &A, const Box<> &B, const Vec3D &posA, const Vec3D &posB, EntityRegistry::ID idA, EntityRegistry::ID idB)
{
    return std::nullopt;
}

}
