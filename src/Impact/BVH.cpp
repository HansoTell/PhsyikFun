#include "EntityRegistry.h"
#include "Impact.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <variant>
#include <vector>


namespace Physik 
{
uint8_t BoundingVolumeHierarchy::FindLongestAxis( const std::vector<EntityBound>& Bounds ) const
{
    assert(!Bounds.empty());

    uint8_t global_Max_Axis;
    double max_Delta = std::numeric_limits<double>().min();

    for( size_t i = 0; i < Bounds[0].Bound.max.size(); ++i )
    {
        double local_min = std::numeric_limits<double>().max();
        double local_max = std::numeric_limits<double>().min();
        for( size_t j = 0; j < Bounds.size(); ++j )
        {
            double axis_max_bound = Bounds[j].Bound.max[i];
            double axis_min_bound = Bounds[j].Bound.min[i];
            if( axis_max_bound > local_max )
                local_max = axis_max_bound;

            if( axis_min_bound < local_min )
                local_min = axis_min_bound;
        }
        double local_delta = local_max - local_min;
        if( local_delta > max_Delta )
        {
            max_Delta = local_delta;
            global_Max_Axis = i;
        }
    }

    return global_Max_Axis;
}
BoundingVolumeHierarchy::BVHNode* BoundingVolumeHierarchy::CreateLeaf( const EntityBound& payload )
{
    m_Nodes.push_back({ payload.ID, payload.Bound, NULL, NULL });
    return &m_Nodes.back();
}

BoundingVolumeHierarchy::BVHNode* BoundingVolumeHierarchy::CreateInternalNode( BVHNode* left, BVHNode* right )
{
    assert(left && right);

    AABB combinedBounds;
    for( size_t i = 0; i < left->Bounds.max.size(); ++i )
    {
        combinedBounds.min[i] = std::min(left->Bounds.min[i], right->Bounds.min[i]);
        combinedBounds.max[i] = std::max(left->Bounds.max[i], right->Bounds.max[i]);
    }

    m_Nodes.emplace_back( std::numeric_limits<EntityRegistry::ID>().max(), combinedBounds, left, right );

    return &m_Nodes.back();
}

BoundingVolumeHierarchy::BVHNode* BoundingVolumeHierarchy::BuildTreeRecursiv( std::span<EntityBound> Entitys, uint8_t axis )
{
    if( Entitys.size() == 1)
        return CreateLeaf(Entitys[0]);

    size_t mid = Entitys.size()/2;
    auto leftSide = Entitys.first(mid);
    auto rightSide = Entitys.last(Entitys.size() - mid);

    BVHNode* left = BuildTreeRecursiv(leftSide, axis);
    BVHNode* right = BuildTreeRecursiv(rightSide, axis);

    return CreateInternalNode(left, right);
}

//TODO: Sollte nicht funktionieren wenn Liste ungerade lang ist
void BoundingVolumeHierarchy::BuildBVH( const EntityRegistry& Entitys, const std::vector<EntityRegistry::ID>& EntitySet )
{
    m_Nodes.clear();
    m_Nodes.reserve(EntitySet.size());

    std::vector<EntityBound> all_bounds;
    all_bounds.reserve(EntitySet.size());

    //Alle Bounds bestimmen
    for( auto& EntityID : EntitySet )
    {
        const auto& ent = Entitys.getById(EntityID);
        AABB bound = std::visit([&](const auto& Shape){
            return getAABB(Shape, ent.getPosition(), ent.getRotation());
        }, ent.getShape());
        all_bounds.push_back({ EntityID, bound });
    }

    //Achse bestimmen
    auto Axis = FindLongestAxis(all_bounds);

    std::sort(all_bounds.begin(), all_bounds.end(), [&](const EntityBound& b1, const EntityBound& b2)
    {
        double midPoint1 = (b1.Bound.max[Axis] + b1.Bound.min[Axis]);
        double midPoint2 = (b2.Bound.max[Axis] + b2.Bound.min[Axis]);

        return midPoint1 < midPoint2;
    });

    //Baum daran rekursiv bauen
    m_root = BuildTreeRecursiv(all_bounds, Axis);
}

void BoundingVolumeHierarchy::FindCollisions( const EntityRegistry& Entitys, std::span<const EntityRegistry::ID> TestEntitys )
{
    for( auto& EntityID : TestEntitys )
    {
        const auto& ent = Entitys.getById(EntityID);
        AABB bound = std::visit([&](const auto& Shape){
            return getAABB(Shape, ent.getPosition(), ent.getRotation());
        }, ent.getShape());


        //find all Collisions


        //algo zum durchgehen, wenn bounding box nicht getreoffen den baum bereich ignorieren, wenn leaf dann detectCollision
        //brauchen auch noch wie wir die Collision zurück geben
        //Wie sieht es mit Gruppen aus können wir das auflösen? --> Können wir das einfach auch über das united Set ding machen
        //Ziehen wir das einfach raus und machen das gleiche??
        //Müssen glaube ich das als erstes Lösen um eine einheitliche Lösung für spartial hash grid und das hier zu finden...
    }
}
}
