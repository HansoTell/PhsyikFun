#pragma once

#include <array>
#include <cstddef>
namespace Physik 
{
template <size_t Dim>
class Vector  
{
public:
    double& at ( size_t i ) { return data.at(i); }
    const double& at( size_t i ) const { return data.at(i); }
    double& operator[]( size_t i ) { return at(i); }
    const double& operator[]( size_t i ) const { return at(i); }
private:
    std::array<double, Dim> data;
};

}
