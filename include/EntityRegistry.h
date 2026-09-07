#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Entity.h"
#include "Vector.h"

namespace Physik 
{
class EntityRegistry
{
public:
    using ID = uint64_t;

    ClassicEntity& getById(ID id){ return m_Entitys[m_IDtoIndex.at(id)]; }
    const ClassicEntity& getById(ID id) const { return m_Entitys[m_IDtoIndex.at(id)]; }
    size_t getEntityIdx(ID id) const { return m_IDtoIndex.at(id); }

    ClassicEntity& at( size_t i ) { return m_Entitys[i]; }
    const ClassicEntity& at( size_t i ) const { return m_Entitys[i]; }
    ClassicEntity& operator[] ( size_t i ) { return at(i); }
    const ClassicEntity& operator[] ( size_t i ) const { return at(i); }

    bool add(ClassicEntity entity)
    {
//FÜr alles an sachen das die nicht ineinander stecken dürfen
/*
        auto it = std::find_if(m_Entitys.begin(), m_Entitys.end(), [&entity](const ClassicEntity& cmpEnt){
            Vec3D diff = entity.getPosition() - cmpEnt.getPosition();

            return diff.EukNorm() <= entity.getRadius() + cmpEnt.getRadius();
        });

        if( it != m_Entitys.end() )
            return false;

*/
        m_IDtoIndex[entity.getID()] = m_Entitys.size();
        m_Entitys.push_back(std::move(entity));

        return true;
    }

    void removeById(ID id)
    {
        auto it = m_IDtoIndex.find(id);
        if( it == m_IDtoIndex.end() ) return;

        size_t idx = it->second;
        size_t lastIdx = m_Entitys.size()-1;

        if( idx != lastIdx )
        {
            std::swap(m_Entitys[idx], m_Entitys[lastIdx]);
            m_IDtoIndex[m_Entitys[idx].getID()] = idx;
        }
        m_Entitys.pop_back();
        m_IDtoIndex.erase(it);
    }

    void removeByIdx(size_t idx)
    {
        auto it = m_IDtoIndex.find(m_Entitys[idx].getID());
        if( it == m_IDtoIndex.end() ) return;

        removeById(it->first);
    }

    auto begin() { return m_Entitys.begin(); }
    auto end() { return m_Entitys.end(); }
    auto begin() const { return m_Entitys.begin(); }
    auto end() const { return m_Entitys.end(); }

    size_t size() const { return m_Entitys.size(); }
    bool empty() const { return m_Entitys.size() == 0; }
    void clear() 
    {
        m_Entitys.clear();
        m_IDtoIndex.clear();
    }
public:
    EntityRegistry() = default;
    EntityRegistry( std::vector<ClassicEntity> entitys ) : m_Entitys(std::move(entitys))
    {
        for( size_t i = 0; i < m_Entitys.size(); ++i )
            m_IDtoIndex[m_Entitys[i].getID()] = i; 
    }
    EntityRegistry(const EntityRegistry&) = default;
    EntityRegistry(EntityRegistry&&) = default;
    ~EntityRegistry() = default;
    EntityRegistry& operator=(const EntityRegistry&) = default;
    EntityRegistry& operator=(EntityRegistry&&) = default;


private:
    std::vector<ClassicEntity> m_Entitys;
    std::unordered_map<ID, size_t> m_IDtoIndex;
};
}
