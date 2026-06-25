#include "Entity.h"
#include "Impact.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace Physik 
{
AdvancedElasticImpact::AdvancedElasticImpact( std::vector<ClassicEntity>& entitys ) : m_Entitys(entitys) 
{ 
    m_EntityStash.reserve(entitys.size());
    m_Components.reserve(entitys.size());
}

const std::array<AdvancedElasticImpact::CellKoords, 25> AdvancedElasticImpact::offsets = 
{
    AdvancedElasticImpact::CellKoords{ -1, 1, 0 },
    AdvancedElasticImpact::CellKoords{ 0, 1, 0 },
    AdvancedElasticImpact::CellKoords{ 1, 1, 0 },

    AdvancedElasticImpact::CellKoords{ -1, 0, 0 },
    AdvancedElasticImpact::CellKoords{ 0, 0, 0 },
    AdvancedElasticImpact::CellKoords{ 1, 0, 0 },

    AdvancedElasticImpact::CellKoords{ -1, -1, -1 },
    AdvancedElasticImpact::CellKoords{ 0, -1, 1 },
    AdvancedElasticImpact::CellKoords{ 1, -1, 1 },

    AdvancedElasticImpact::CellKoords{ -1, 0, 1 },
    AdvancedElasticImpact::CellKoords{ 0, 0, 1 },
    AdvancedElasticImpact::CellKoords{ 1, 0, 1 },

    AdvancedElasticImpact::CellKoords{ -1, 1, 1 },
    AdvancedElasticImpact::CellKoords{ 0, 1, 1 },
    AdvancedElasticImpact::CellKoords{ 1, 1, 1 }
};

static void DepthSearth( std::vector<const ClassicEntity*>& entitys, std::vector<std::vector<const ClassicEntity*>>& outComponents )
{
    std::vector<const ClassicEntity*> Stack;
    Stack.reserve(entitys.size());

    while( !entitys.empty() )
    {
        auto StartingEntity = entitys.back();
        entitys.pop_back();

        Stack.push_back(StartingEntity);
        outComponents.push_back( { StartingEntity } );

        auto& Compnent = outComponents.back();
        Compnent.reserve(entitys.size());

        while (!Stack.empty()) 
        {
            const ClassicEntity* Currnode = Stack.back();
            Stack.pop_back();

            for( auto& ent : entitys )
            {
                Vec3D diff = ent->getPosition() - Currnode->getPosition();
                if( diff.EukNorm() > ent->getRadius() + Currnode->getRadius() )
                    continue;

                Stack.push_back(ent);
                Compnent.push_back(ent);

                auto it = std::find(entitys.begin(), entitys.end(), ent);
                entitys.erase(it);
            }
        }
    }
}

void AdvancedElasticImpact::ApplyImpacts( SimulationState& state )
{
    using  CellKoords = AdvancedElasticImpact::CellKoords;
    m_CellSize = 2.0 * FindMaxRadius() + 1.0;

    BuildMap();
    
    //Hier ist jetzt das Problem nur noch doppelte komponenten... Also wenn bei einem die nicht drinnenn waren das die zusammen gepackt werden
    
    for( auto&[Cell, Entitys] : m_Cells )
    {
        for( const auto& offset : AdvancedElasticImpact::offsets)
        {
            CellKoords neighbour { Cell.x_Koord + offset.x_Koord, Cell.y_Koord+offset.y_Koord, Cell.z_Koord+offset.z_Koord };

            auto& cellEntitys = m_Cells[neighbour];
            for( auto entity : cellEntitys ) m_EntityStash.push_back(entity);
        }
        DepthSearth(m_EntityStash, m_Components);
        m_EntityStash.clear();
    }


    for( auto& component : m_Components )
    {
        if(component.size() == 1)
            continue;
        if(component.size() == 2)
        {
            //einfache Auflösung
        }
        if( component.size() > 2 )
        {

        }

        switch (component.size()) 
        {
        case 1: 
            break;
        case 2:
        {
            //einfache auflösung
            break;
        }
        default:
        {
            for( int i = 0; i < 10; ++i )
            {
                //aulflösen

            }
        }
        }
    }
    //Bessere anpassung an drift weg
}

double AdvancedElasticImpact::FindMaxRadius() const
{
    double MaxRadius = std::numeric_limits<double>::min();
    for( size_t i = 0; i < m_Entitys.size(); ++i )
    {
        auto rad = m_Entitys[i].getRadius();
        if( rad > MaxRadius) MaxRadius = rad;
    }

    return MaxRadius;
}

void AdvancedElasticImpact::BuildMap()
{
    size_t avrg_bucketsize = m_Entitys.size()/4;

    m_Cells.clear();
    for( const auto& ent : m_Entitys )
    {
        auto CellKey = CellKoords::getCell(ent.getPosition());

        if(m_Cells.find(CellKey) == m_Cells.end())
            m_Cells[CellKey].reserve(avrg_bucketsize);

        m_Cells[CellKey].push_back(&ent);
    }
}
    
}
