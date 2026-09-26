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

static double findShortestPointToSurface( const Vector<6, double> distances, const Vec3D& centerLocal, const Vec3D& BoxHalfSizes, Vec3D& outNormalLocal, Vec3D& outContactLocal )
{
    double minDistane = std::numeric_limits<double>::infinity();
    for( size_t i = 0; i < distances.size(); ++i )
    {
        double distI = distances[i];
        if( distI < minDistane )
        {
            minDistane = distI;

            for( size_t j = 0; j < outContactLocal.size(); ++j )
            {
                if( (i%3) != j )
                {
                    outNormalLocal[j] = 0.0;
                    outContactLocal[j] = centerLocal[j];
                }else {
                    outNormalLocal[j] = (i < distances.size()/2) ? 1.0 : -1.0;
                    outContactLocal[j] = (i < distances.size()/2) ? BoxHalfSizes[j] : -BoxHalfSizes[j];
                }
            }
        }
    }
    return minDistane;
}

std::optional<CollisionManifold> detectCollision(const Sphere<double>& A, const Box<>& B,  const ClassicEntity& entA, const ClassicEntity& entB)
{
    const Vec3D& posA = entA.getPosition();
    const Vec3D& posB = entB.getPosition();

    Vec3D d = posA - posB;

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

    const constexpr double GeometryEpsilon = 1e-9;
    if( diffSquared > GeometryEpsilon * GeometryEpsilon )
    {
        double distance = std::sqrt(diffSquared);
        normalLocal = diffrence / distance;
        penetration = A.m_Radius - distance;
    } else {
        Vec3D distancePos = B.halfSize - localD;
        Vec3D distanceNeg= B.halfSize + localD;
        //Brauchen 6D Vektor erst Neg dann Pos
        Vector<6, double> distances;
        for( size_t i = 0; i < distanceNeg.size(); ++i ) distances[i] = distanceNeg[i];
        for( size_t i = 0; i < distancePos.size(); ++i ) distances[i + distanceNeg.size()] = distancePos[i];

        double minDistance = findShortestPointToSurface(distances, localD, B.halfSize, normalLocal, conatctLocal);

        penetration = A.m_Radius + minDistance;
    }

    Vec3D contactPoint = posB + entB.getRotation().Rotate(conatctLocal);
    Vec3D normal = entB.getRotation().Rotate(normalLocal);

    return CollisionManifold{ entA.getID(), entB.getID(), penetration, normal, contactPoint };
}

std::optional<CollisionManifold> detectCollision(const Box<>& A, const Sphere<>& B, const ClassicEntity& entA, const ClassicEntity& entB)
{
    return detectCollision(B, A, entB, entA);
}

//TODO:
std::optional<CollisionManifold> detectCollision(const Box<> &A, const Box<> &B, const ClassicEntity& entA, const ClassicEntity& entB)
{
    return std::nullopt;
}

}
