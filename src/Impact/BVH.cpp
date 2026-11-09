#include "Impact.h"
#include <cassert>
#include <cstddef>
#include <limits>
#include <variant>
#include <vector>


namespace Physik 
{

uint8_t BoundingVolumeHierarchy::FindLongestAxis( const std::vector<AABB>& Bounds ) const
{
    assert(!Bounds.empty());

    uint8_t global_Max_Axis;
    double max_Delta = std::numeric_limits<double>().min();

    for( size_t i = 0; i < Bounds[0].max.size(); ++i )
    {
        double local_min = std::numeric_limits<double>().max();
        double local_max = std::numeric_limits<double>().min();
        for( size_t j = 0; j < Bounds.size(); ++j )
        {
            double axis_max_bound = Bounds[j].max[i];
            double axis_min_bound = Bounds[j].min[i];
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

void BoundingVolumeHierarchy::BuildBVH( const EntityRegistry& Entitys, const std::vector<EntityRegistry::ID>& EntitySet )
{
    std::vector<AABB> all_bounds;
    all_bounds.reserve(EntitySet.size());

    //Alle Bounds bestimmen
    for( auto& EntityID : EntitySet )
    {
        const auto& ent = Entitys.getById(EntityID);
        AABB bound = std::visit([&](const auto& Shape){
            return getAABB(Shape, ent.getPosition(), ent.getRotation());
        }, ent.getShape());
        all_bounds.push_back(bound);
    }

    //Achse bestimmen
    auto Axis = FindLongestAxis(all_bounds);

    //An Achse sortieren
    //Baum daran rekursiv bauen
    //
    //-> wie verbinden wir entity und bounding box muss ja zusammen passen
}
}
