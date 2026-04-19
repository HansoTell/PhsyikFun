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
    void minus( const Vector<Dim> other ) { for( int i = 0; i < Dim; i++ ) at(i) -= other[i]; }
    void add( const Vector<Dim> other ) { for( int i = 0; i < Dim; i++ ) at(i) += other[i]; }
    void skalarProduct( double skalar ) { for( int i = 0; i < Dim; i++ ) at(i) *= skalar; }
    double& operator[]( size_t i ) { return at(i); }
    const double& operator[]( size_t i ) const { return at(i); }
    void operator-(const Vector<Dim>& other) { minus(other); }
    void operator+(const Vector<Dim>& other) { add(other); }
    void operator*( double skalar ) { skalarProduct(skalar); }
private:
    std::array<double, Dim> data;
};


using Vec2D = Vector<2>;
using Vec3D = Vector<3>;

}
