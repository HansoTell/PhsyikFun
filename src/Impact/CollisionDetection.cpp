#include "CollisionDetction.h"

#include "Entity.h"
#include "Impact.h"
#include "Math/Math.h"
#include "Vector.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>

namespace Physik 
{

static const constexpr double GeometryEpsilon = 1e-9;


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

static constexpr size_t dim = 3;
static std::array<Vec3D, dim> GetBodyFixedAxis( const ClassicEntity& Entity )
{
    std::array<Vec3D, dim> erg;
    for( size_t i = 0; i < dim; ++i )
    {
        Vec3D GlobalAxis;
        GlobalAxis[i] = 1.0;
        Vec3D localAxis = Entity.getRotation().Rotate(GlobalAxis);
        erg[i] = localAxis;
    }
    return erg;
}

static double GetR( const ClassicEntity& ent, const Vec3D& Axis, const Box<>& Shape, const std::array<Vec3D, dim>& localAxis )
{
    double erg = 0.0;
    for( size_t i = 0; i < Axis.size(); ++i )
         erg += Shape.halfSize[i] * std::fabs(Math::VectorCalc::VectorProduct(localAxis[i], Axis));

    return erg;
}

std::optional<CollisionManifold> detectCollision(const Box<> &A, const Box<> &B, const ClassicEntity& entA, const ClassicEntity& entB)
{
    using namespace ::Math::VectorCalc;

    //Axis Calculation
    std::array<Vec3D, dim> localAxisA = GetBodyFixedAxis(entA);
    std::array<Vec3D, dim> localAxisB = GetBodyFixedAxis(entB);
    std::array<Vec3D, 3*dim> localCrossProductAxis; 

    for( size_t i = 0; i < localAxisA.size(); ++i )
        for( size_t j = 0; j < localAxisB.size(); ++j )
            localCrossProductAxis[localAxisA.size()*i+j] = CrossProduct(localAxisA[i], localAxisB[j]);

    std::array<Vec3D, localAxisA.size() + localAxisB.size() + localCrossProductAxis.size()> AllAxis;

    auto it = AllAxis.begin();
    it = std::copy(localAxisA.begin(), localAxisA.end(), it);
    it = std::copy(localAxisB.begin(), localAxisB.end(), it);
    it = std::copy(localCrossProductAxis.begin(), localCrossProductAxis.end(), it);

    //Test If Collision
    Vec3D CenterDiff = entB.getPosition() - entA.getPosition(); 
    size_t AxisMinPenetration;
    double minPenetration = std::numeric_limits<double>::infinity();

    for( size_t i = 0; i < AllAxis.size(); ++i )
    {
        if( AllAxis[i].BetragsQuadrat() < GeometryEpsilon * GeometryEpsilon )
            continue;

        double rA = GetR(entA, AllAxis[i], A, localAxisA);
        double rB = GetR(entB, AllAxis[i], B, localAxisB);
        double r = rA + rB;
        double diff = std::fabs(VectorProduct(CenterDiff, AllAxis[i]));

        if( diff > r )
            return std::nullopt;

        double Penetration = r - diff;
        if( minPenetration < Penetration )
        {
            minPenetration = Penetration;
            AxisMinPenetration = i;
        }
    }

    Vec3D normal = (VectorProduct(CenterDiff, AllAxis[AxisMinPenetration]) >= 0) ? AllAxis[AxisMinPenetration] : -1 * AllAxis[AxisMinPenetration];

    return CollisionManifold{ entA.getID(), entB.getID(), minPenetration, normal };
}
}
