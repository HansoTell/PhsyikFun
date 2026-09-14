#pragma once

#include <cmath>
#include <memory>
namespace Physik 
{
template<typename T = double>
struct Material 
{
    T Restitution;
};

template<typename T = double>
class IMaterialCombiner
{
public:
    virtual ~IMaterialCombiner() = default;
    virtual Material<T> CombineMaterials( const Material<T>& first, const Material<T>& secound ) const = 0;
    virtual std::unique_ptr<IMaterialCombiner<T>> clone() const = 0;
};

//Geometrische Mittel
template<typename T = double>
class BasicMaterialCombiner : public IMaterialCombiner<T>
{
public:
    Material<T> CombineMaterials( const Material<T>& first, const Material<T>& secound ) const override
    {
        return { std::sqrt(first.Restitution * secound.Restitution) };
    }
    std::unique_ptr<IMaterialCombiner<T>> clone() const override
    {
        return std::make_unique<BasicMaterialCombiner<T>>();
    }


};
    
}
