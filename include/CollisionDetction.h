#include "Shapes.h"
#include "Vector.h"
#include <optional>

namespace Physik 
{
    struct CollisionPair;


    std::optional<CollisionPair> detectCollision(const Sphere<double>& A, const Sphere<double>& B, const Vec3D& posA, const Vec3D& posB); 
    std::optional<CollisionPair> detectCollision(const Sphere<double>& A, const Box<>& B, const Vec3D& posA, const Vec3D& posB); 
    std::optional<CollisionPair> detectCollision(const Box<>& A, const Box<>& B, const Vec3D& posA, const Vec3D& posB); 
    std::optional<CollisionPair> detectCollision(const Box<>& A, const Sphere<>& B, const Vec3D& posA, const Vec3D& posB); 
}
