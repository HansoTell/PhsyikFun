#include "Entity.h"
#include "Shapes.h"
#include <optional>

namespace Physik 
{
    struct CollisionManifold;


    std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Sphere<double>& B, const ClassicEntity& entA, const ClassicEntity& entB);
    std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Box<>& B, const ClassicEntity& entA, const ClassicEntity& entB);
    std::optional<CollisionManifold> detectCollision(const Box<>& A, const Box<>& B,  const ClassicEntity& entA, const ClassicEntity& entB);
    std::optional<CollisionManifold> detectCollision(const Box<>& A, const Sphere<>& B,  const ClassicEntity& entA, const ClassicEntity& entB);
}
