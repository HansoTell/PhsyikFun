#include "PropertyCalculus.h"
#include "Vector.h"
#include <cstddef>


namespace Physik 
{
void ClassicPropCalc::ApplyAllAcceleration( const std::vector<ClassicEntity>& AllEntitys, std::vector<ClassicEntityState>& outPropertys ) const 
{
    for( size_t i = 0; i < AllEntitys.size(); i++ )
        outPropertys[i].m_Acceleration = outPropertys[i].m_Force * (1/AllEntitys[i].getMass());
}
void ClassicPropCalc::ApplyAllNewtonIntegration( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const 
{

}
void ClassicPropCalc::ApplyAllKineticEnergy( const std::vector<ClassicEntity>& entitys, std::vector<ClassicEntityState>& outPropertys ) const 
{
    for( size_t i = 0; i < entitys.size(); i++ )
    {
        auto& property = outPropertys[i];
        property.KineticEnergy = 0.5 * entitys[i].getMass() * property.m_Velocity.EukNorm() * property.m_Velocity.EukNorm();
    }
}

}
