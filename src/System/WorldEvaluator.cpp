#include "SystemCore.h"
#include "Vector.h"
#include <cassert>
#include <climits>
#include <cstddef>
#include <vector>

namespace Physik 
{

//TODO:
WorldEvaluator::WorldEvaluator(){}
WorldEvaluator::WorldEvaluator( const WorldEvaluator& other ){}
WorldEvaluator::WorldEvaluator( WorldEvaluator&& other ){}



void WorldEvaluator::CalcAccelerations( const SimulationState& state, double Time, Vec3D outAccelerations[] ) const 
{
    assert(outAccelerations != NULL);
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

    for( size_t i = 0; i < m_ExtPotentials.size(); i++ )
    {
        for( size_t j = 0; j < state.size(); j++ )
        {
            const auto& ent1 = state[j];
            Vec3D force = m_ExtPotentials[i].getForce(ent1.getEntityState(), Time);
            outAccelerations[i] += force;
        }
    }

    for( size_t i = 0; i < state.size(); i++ )
        outAccelerations[i] /= state[i].getMass();
}

void WorldEvaluator::CalcPotEnergy( const SimulationState& state, double Time, double outEpots[] ) const 
{
    assert(outEpots != NULL);
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
            outEpots[i] += E;
        }
    }
}

void WorldEvaluator::CalcKineticEnergy( const SimulationState& state, double Time, double outEKins[] ) const 
{
    assert(outEKins != NULL);
    for( size_t i = 0; i < state.size(); i++ )
        outEKins[i] = 0.5 * state[i].getMass() * state[i].getVelocity().BetragsQuadrat();
}

void WorldEvaluator::UpdateAccelerations( SimulationState& state, double Time ) 
{
    Vec3D newAccelerations[state.size()];
    CalcAccelerations(state, Time, newAccelerations);

    for( size_t i = 0; i < state.size(); i++ )
        state[i].setAcceleration(newAccelerations[i]);
}
void WorldEvaluator::UpdateKineticEnergys( SimulationState& state, double Time ) 
{
    double newEkins[state.size()];
    CalcKineticEnergy(state, Time, newEkins);

    for( size_t i = 0; i < state.size(); i++ )
        state[i].setKineticEnergy(newEkins[i]);
}
void WorldEvaluator::UpdatePotentialEnergys( SimulationState& state, double Time ) 
{
    double newEpots[state.size()];
    CalcPotEnergy(state, Time, newEpots);

    for( size_t i = 0; i < state.size(); i++ )
        state[i].setKineticEnergy(newEpots[i]);
}


void WorldEvaluator::addExternPotential( ClassicField potential ) { m_ExtPotentials.push_back(ClassicField(std::move(potential))); }
void WorldEvaluator::addMulitpleExternPotentials( std::vector<ClassicField> potentials ) { for( int i = 0; i < potentials.size(); i++ ) m_ExtPotentials.push_back(std::move(potentials[i])); }
void WorldEvaluator::addEntityPotential( ClassicInteraction potential) { m_EntityPotentials.push_back(std::move(potential)); }
void WorldEvaluator::addMultipleEntityPotentials( std::vector<ClassicInteraction> potentials ) { for( size_t i = 0; i < potentials.size(); i++) m_EntityPotentials.push_back(std::move(potentials[i])); }
} 
