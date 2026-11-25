#include "Impact.h"
#include "Datastructures/DisjointSets.h"

#include <vector>


namespace Physik 
{

 std::vector<std::vector<CollisionManifold>> ConnectedParts::getConnectedParts( const EntityRegistry& Entitys, const CollisionSet& Collisions ) 
{
    ds::DisjointSets Union(Entitys.size());
    for( auto& pair : Collisions)
    {
        size_t ent1Idx = Entitys.getEntityIdx(pair.ent1);
        size_t ent2Idx = Entitys.getEntityIdx(pair.ent2);

        Union.unite(ent1Idx, ent2Idx);
    }

    std::unordered_map<size_t, std::vector<size_t>> groups = Union.getSets();
    std::vector<std::vector<CollisionManifold>> KollisionGroups(groups.size());
    //O(groups.size)
    size_t idx = 0;
    for( auto&[_, group] : groups )
    {
        KollisionGroups[idx].reserve(group.size());
        for( size_t i = 0; i < group.size(); ++i )
        {
            const auto ent1ID = Entitys.at(group[i]).getID();
            for( size_t j = i+1; j < group.size(); ++j )
            {
                const auto ent2ID = Entitys.at(group[j]).getID();
                CollisionManifold hash = (ent1ID < ent2ID) ? CollisionManifold{ ent1ID, ent2ID, 0.0, Vec3D(), Vec3D() } : CollisionManifold{ ent2ID, ent1ID, 0.0, Vec3D(), Vec3D() };
                if( auto it = Collisions.find(hash); it != Collisions.cend() ) KollisionGroups[idx].push_back(*it);
            }
        }
        ++idx;
    }

    return KollisionGroups;
}
    
}
