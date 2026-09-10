#include "Shapes.h"
#include "Vector.h"
#include "EntityRegistry.h"
#include <optional>

namespace Physik 
{
    struct CollisionManifold;


    std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Sphere<double>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB); 
    std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Box<>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB); 
    std::optional<CollisionManifold> detectCollision(const Box<>& A, const Box<>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB); 
    std::optional<CollisionManifold> detectCollision(const Box<>& A, const Sphere<>& B, const Vec3D& posA, const Vec3D& posB, EntityRegistry::ID idA, EntityRegistry::ID idB); 
}
