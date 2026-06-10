#include "Entity.h"
#include <vector>

namespace Physik
{
using SimulationState = std::vector<ClassicEntity>;


class IImpactEvaluator  
{
public:
    virtual ~IImpactEvaluator() = default;
    virtual void ApplyImpacts( SimulationState& state ) = 0;
};

class ElasticImpact : public IImpactEvaluator 
{
public:
    void ApplyImpacts( SimulationState& state ) override;
public:
    ElasticImpact() = default;
    ElasticImpact( const ElasticImpact& other ) = default;
    ElasticImpact( ElasticImpact&& other ) = default;
    ~ElasticImpact() = default;
};
}
