#include "CollisionDetction.h"

#include "Impact.h"
#include "Vector.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>

namespace Physik 
{
std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Sphere<double>& B,  const ClassicEntity& entA, const ClassicEntity& entB)
{
    Vec3D diff =  entB.getPosition() - entA.getPosition();

    if( diff.EukNorm() > A.m_Radius + B.m_Radius ) 
        return std::nullopt;

    Vec3D normal = diff/diff.EukNorm();
    double penetration = A.m_Radius + B.m_Radius - diff.EukNorm();

    return CollisionManifold{ entA.getID(), entB.getID(), penetration, normal };
}

static Vec3D ProjectKoordOnBox( const Vec3D& BoxMin, const Vec3D& BoxMax, const Vec3D& posSphere )
{
    return Vec3D{ std::max(BoxMin.at(0), std::min(posSphere.at(0), BoxMax.at(0))), std::max(BoxMin.at(1), std::min(posSphere.at(1), BoxMax.at(1))), std::max(BoxMin.at(2), std::min(posSphere.at(2), BoxMax.at(2))) };
}

std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Box<>& B,  const ClassicEntity& entA, const ClassicEntity& entB)
{
    const Vec3D& posA = entA.getPosition();
    const Vec3D& posB = entB.getPosition();

    Vec3D d = posB - posA;

    Vec3D localD = entB.getRotation().Inverse().Rotate(d);

    Vec3D LocalnearestPoint { 
        std::clamp(localD[0], -B.halfSize[0], B.halfSize[0]), 
        std::clamp(localD[1], -B.halfSize[1], B.halfSize[1]), 
        std::clamp(localD[2], -B.halfSize[2], B.halfSize[2])
    };

    Vec3D diffrence = localD - LocalnearestPoint; 
    double diffSquared = diffrence.BetragsQuadrat();
    if( diffSquared > A.m_Radius * A.m_Radius )
        return std::nullopt;

    Vec3D normalLocal;
    double penetration;
    Vec3D conatctLocal = LocalnearestPoint;
//TODO:
    if( true )
    {
        double distance = std::sqrt(diffSquared);
        normalLocal = diffrence / distance;
        penetration = A.m_Radius - distance;
    } else {
        auto distancePos = B.halfSize - localD;
        auto distanceNeg= B.halfSize + localD;

        double minDistane = std::numeric_limits<double>::infinity();
        for( size_t i = 0; i < distancePos.size(); ++i )
        {
            double distI = distancePos[i];
            if( distI < minDistane )
            {
                minDistane = distI;
//normalLoca setzten --> if einfach ein array wo entsprechende local genommen wird
//Pain IG wenn man Permutation generieren könnte gerade wäre es möglich das trotzdem so umzusetzten was immerhin ein bisschen besser ist. Minus auch machbar reihenfolge schwierig
//--> Also leerer Vec3D erstellen und dann durchlaufen wenn i != id dann setzten wir da local x sonst setzten wir da den anderen wert ein. -> Hilfsmethode das zu viel hierfür



            }

        }
    
    }

    Vec3D contactPoint = posB + entB.getRotation().Rotate(conatctLocal);
    Vec3D normal = entB.getRotation().Rotate(normalLocal);

    return CollisionManifold{ entA.getID(), entB.getID(), penetration, normal, contactPoint };
}

std::optional<CollisionManifold> detectCollision(const Box<>& A, const Sphere<>& B, const ClassicEntity& entA, const ClassicEntity& entB)
{
    return detectCollision(B, A, entA, entB);
}

//TODO:
std::optional<CollisionManifold> detectCollision(const Box<> &A, const Box<> &B, const ClassicEntity& entA, const ClassicEntity& entB)
{
    return std::nullopt;
}

}
