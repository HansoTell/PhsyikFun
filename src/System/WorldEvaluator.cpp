#include "Evaluator.h"
#include "SystemCore.h"
#include "Vector.h"
#include <cassert>
#include <climits>
#include <cstddef>
#include <vector>

namespace Physik 
{
static constexpr size_t default_reserve_size = 10; 

WorldEvaluator::WorldEvaluator() 
{
    m_ExtPotentials.reserve(default_reserve_size);
    m_EntityPotentials.reserve(default_reserve_size);
    m_NonPotForce.reserve(default_reserve_size);
}
WorldEvaluator::WorldEvaluator( std::vector<ClassicField> extFields ) : m_ExtPotentials(std::move(extFields))
{
    m_EntityPotentials.reserve(default_reserve_size);
    m_NonPotForce.reserve(default_reserve_size);
}
WorldEvaluator::WorldEvaluator( std::vector<ClassicInteraction> entityFields ) : m_EntityPotentials(std::move(entityFields))
{
    m_EntityPotentials.reserve(default_reserve_size);
    m_NonPotForce.reserve(default_reserve_size);
}
WorldEvaluator::WorldEvaluator( std::vector<ClassicNonPotentialForce> NonPotentialForces ) : m_NonPotForce(std::move(NonPotentialForces))
{
    m_ExtPotentials.reserve(default_reserve_size);
    m_NonPotForce.reserve(default_reserve_size);
}
WorldEvaluator::WorldEvaluator( std::vector<ClassicField> extFields, std::vector<ClassicInteraction> entityFields ) : m_ExtPotentials(std::move(extFields)), m_EntityPotentials(std::move(entityFields)){ m_NonPotForce.reserve(default_reserve_size); }
WorldEvaluator::WorldEvaluator( std::vector<ClassicField> extFields, std::vector<ClassicNonPotentialForce> NonPotentialForces ) : m_ExtPotentials(std::move(extFields)), m_NonPotForce(std::move(NonPotentialForces)) { m_EntityPotentials.reserve(default_reserve_size); }
WorldEvaluator::WorldEvaluator( std::vector<ClassicInteraction> entityFields, std::vector<ClassicNonPotentialForce> NonPotentialForces ) : m_EntityPotentials(std::move(entityFields)), m_NonPotForce(std::move(NonPotentialForces)) { m_ExtPotentials.reserve(default_reserve_size); }
WorldEvaluator::WorldEvaluator( std::vector<ClassicField> extFields, std::vector<ClassicInteraction> entityFields, std::vector<ClassicNonPotentialForce> NonPotentialForces ) : m_ExtPotentials(std::move(extFields)), m_EntityPotentials(std::move(entityFields)), m_NonPotForce(std::move(NonPotentialForces)){}
WorldEvaluator::WorldEvaluator( const WorldEvaluator& other ) :
    m_ExtPotentials(other.m_ExtPotentials),
    m_EntityPotentials(other.m_EntityPotentials),
    m_NonPotForce(other.m_NonPotForce){}
WorldEvaluator::WorldEvaluator( WorldEvaluator&& other ) : 
    m_ExtPotentials(std::move(other.m_ExtPotentials)), 
    m_EntityPotentials(std::move(other.m_EntityPotentials)),
    m_NonPotForce(std::move(other.m_NonPotForce)){}


void WorldEvaluator::CalcAccelerations( const SimulationState& state, double Time, std::vector<Vec3D>& outAccelerations ) const 
{
    //Entity Potentials
    for( auto& interaction : m_EntityPotentials )
    {
        for( size_t i = 0; i < state.size(); i++ )
        {
            const auto& ent1 = state[i];
            for( size_t j = i+1; j < state.size(); j++ )
            {
                Vec3D force = interaction.getForce(ent1.getEntityState(), state[j].getEntityState(), Time);

                outAccelerations[i] += force;
                outAccelerations[j] -= force;
            }
        }
    }

    //Extern Potentials
    for( auto& field : m_ExtPotentials )
    {
        for( size_t i = 0; i < state.size(); i++ )
        {
            const auto& ent1 = state[i];
            Vec3D force = field.getForce(ent1.getEntityState(), Time);
            outAccelerations[i] += force;
        }
    }

    //Non Potential Forces
    for( auto& NonPotF : m_NonPotForce )
    {
        for( size_t i = 0; i < state.size(); i++ )
        {
            const auto& ent1 = state[i];
            Vec3D force = NonPotF.getForce(ent1.getEntityState(), Time);
            outAccelerations[i] += force;
        }
    }

    for( size_t i = 0; i < state.size(); i++ )
        outAccelerations[i] /= state[i].getMass();

}

void WorldEvaluator::CalcPotEnergy( const SimulationState& state, double Time, std::vector<double>& outEpots ) const 
{
    for( auto& interaction : m_EntityPotentials )
    {
        for( size_t i = 0; i < state.size(); i++ )
        {
            const auto& ent1 = state[i];
            for( size_t j = i+1; j < state.size(); j++ )
            {
                double E = interaction.getPotentialEnergy(ent1.getEntityState(), state[j].getEntityState(), Time);

                outEpots[i] += E;
                outEpots[j] -= E;
            }
        }
    }

    for( size_t i = 0; i < m_ExtPotentials.size(); i++ )
    {
        for( size_t j = 0; j < state.size(); j++ )
        {
            const auto& ent1 = state[j];
            double E = m_ExtPotentials[i].getPotentialEnergy(ent1.getEntityState(), Time);
            outEpots[j] += E;
        }
    }
}

void WorldEvaluator::CalcKineticEnergy( const SimulationState& state, std::vector<double>& outEKins ) const 
{
    for( size_t i = 0; i < state.size(); i++ )
        outEKins[i] = 0.5 * state[i].getMass() * state[i].getVelocity().BetragsQuadrat();
}

void WorldEvaluator::UpdateAccelerations( SimulationState& state, double Time ) 
{
    m_AccScratch.assign(state.size(), Vec3D{});
    CalcAccelerations(state, Time, m_AccScratch);

    for( size_t i = 0; i < state.size(); i++ )
        state[i].setAcceleration(m_AccScratch[i]);
}
void WorldEvaluator::UpdateKineticEnergys( SimulationState& state ) 
{
    m_EkinScratch.assign(state.size(), double{});
    CalcKineticEnergy(state, m_EkinScratch);

    for( size_t i = 0; i < state.size(); i++ )
        state[i].setKineticEnergy(m_EkinScratch[i]);
}
void WorldEvaluator::UpdatePotentialEnergys( SimulationState& state, double Time ) 
{
    m_EPotScratch.assign(state.size(), double{});
    CalcPotEnergy(state, Time, m_EkinScratch);

    for( size_t i = 0; i < state.size(); i++ )
        state[i].setPotentialEnergy(m_EkinScratch[i]);
}

void WorldEvaluator::addExternPotential( ClassicField potential ) { m_ExtPotentials.push_back(ClassicField(std::move(potential))); }
void WorldEvaluator::addMulitpleExternPotentials( std::vector<ClassicField> potentials ) { for( int i = 0; i < potentials.size(); i++ ) m_ExtPotentials.push_back(std::move(potentials[i])); }
void WorldEvaluator::addEntityPotential( ClassicInteraction potential) { m_EntityPotentials.push_back(std::move(potential)); }
void WorldEvaluator::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) { for( size_t i = 0; i < potentials.size(); i++) m_EntityPotentials.push_back(std::move(potentials[i])); }
void WorldEvaluator::addNonPotentialForce( ClassicNonPotentialForce force ) { m_NonPotForce.push_back(std::move(force)); }
void WorldEvaluator::addMulitpleNonPotentialForce( std::vector<ClassicNonPotentialForce> force ) { for( size_t i = 0; i < m_NonPotForce.size(); i++ ) m_NonPotForce.push_back(std::move(force[i])); } 
} 
