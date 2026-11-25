#include "AABB.h"
#include "EntityRegistry.h"
#include "CollisionDetction.h"
#include "Impact.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <span>
#include <variant>
#include <vector>


namespace Physik 
{
BoundingVolumeHierarchy::Axis BoundingVolumeHierarchy::FindLongestAxis( std::span<const EntityBound> Bounds ) const
{
    assert(!Bounds.empty());

    uint8_t global_Max_Axis;
    double max_Delta = std::numeric_limits<double>().lowest();

    for( size_t i = 0; i < Bounds[0].Bound.max.size(); ++i )
    {
        double local_min = std::numeric_limits<double>().max();
        double local_max = std::numeric_limits<double>().lowest();
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

BoundingVolumeHierarchy::BVHNode* BoundingVolumeHierarchy::BuildTreeRecursiv( std::span<EntityBound> Entitys )
{
    if( Entitys.size() == 1)
        return CreateLeaf(Entitys[0]);

    //Achse bestimmen
    Axis axis = FindLongestAxis(Entitys);

    std::sort(Entitys.begin(), Entitys.end(), [&](const EntityBound& b1, const EntityBound& b2)
    {
        double midPoint1 = (b1.Bound.max[axis] + b1.Bound.min[axis]);
        double midPoint2 = (b2.Bound.max[axis] + b2.Bound.min[axis]);

        return midPoint1 < midPoint2;
    });

    size_t mid = Entitys.size()/2;
    auto leftSide = Entitys.first(mid);
    auto rightSide = Entitys.last(Entitys.size() - mid);

    BVHNode* left = BuildTreeRecursiv(leftSide);
    BVHNode* right = BuildTreeRecursiv(rightSide);

    return CreateInternalNode(left, right);
}

void BoundingVolumeHierarchy::BuildBVH( const EntityRegistry& Entitys, const std::vector<EntityRegistry::ID>& EntitySet )
{
    if( m_Nodes.size() == 2*EntitySet.size()-1)
        return;

    m_Nodes.clear();
    m_Nodes.reserve(2*EntitySet.size()-1);

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

    //Baum daran rekursiv bauen
    m_root = BuildTreeRecursiv(all_bounds);
}

void BoundingVolumeHierarchy::QueryBVH( BVHNode* Node, AABB bound, const EntityRegistry& Entitys, EntityRegistry::ID TestEntityID, CollisionSet& outCollisions ) 
{
    if( Node->isLeaf() && isAABBsOverlapping(Node->Bounds, bound) )
    {
        const auto& TestEnt = Entitys.getById(TestEntityID);
        const auto& TreeEnt = Entitys.getById(Node->EntityID);

        auto collision_or = FindCollision(TestEnt, TreeEnt);

        if( collision_or.has_value() )
        {
            auto& collision = collision_or.value();

            assert(outCollisions.find(collision) == outCollisions.end());
            outCollisions.insert(collision);
        }
        return;
    }

    if( isAABBsOverlapping(Node->Bounds, bound) )
    {
        QueryBVH(Node->left, bound, Entitys, TestEntityID, outCollisions);
        QueryBVH(Node->right, bound, Entitys, TestEntityID, outCollisions);
    }

    return;
}

CollisionSet BoundingVolumeHierarchy::CollectCollisions( const EntityRegistry& Entitys, std::span<const EntityRegistry::ID> TestEntitys )
{
    CollisionSet Collisions;
    Collisions.reserve(TestEntitys.size());
    for( auto& EntityID : TestEntitys )
    {
        const auto& ent = Entitys.getById(EntityID);
        AABB bound = std::visit([&](const auto& Shape){
            return getAABB(Shape, ent.getPosition(), ent.getRotation());
        }, ent.getShape());

        //find all Collisions
        QueryBVH(m_root, bound, Entitys, EntityID, Collisions);
    }

    return Collisions;
}
}
