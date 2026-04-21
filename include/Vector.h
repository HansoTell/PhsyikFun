#pragma once

#include <array>
#include <cmath>
#include <complex>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

//interaces für noemen und skalarprodukte??
namespace Physik 
{
template <size_t Dim = 3, typename T = double>
class Vector  
{
static_assert(std::is_arithmetic_v<T>);
public:
    double& at ( size_t i ) { return data.at(i); }
    const double& at( size_t i ) const { return data.at(i); }
    void minusIP( const Vector<Dim, T> other ) { for( int i = 0; i < Dim; i++ ) at(i) -= other[i]; }
    //wie mache ichd das ? Also wie retune ich die sachen in die initialliser list?
    Vector<Dim, T> minusOP( const Vector<Dim, T> other ){}
    void add( const Vector<Dim, T> other ) { for( int i = 0; i < Dim; i++ ) at(i) += other[i]; }
    void skalarProduct( double skalar ) { for( int i = 0; i < Dim; i++ ) at(i) *= skalar; }
    double EukNorm() const 
    { 
        double betragsquadrat = 0;
        for( int i = 0; i < Dim; i++ )
        {
            betragsquadrat += data[i] * data[i];

        }
        return sqrt(betragsquadrat); 
    }
    double& operator[]( size_t i ) { return at(i); }
    const double& operator[]( size_t i ) const { return at(i); }
    void operator-(const Vector<Dim, T>& other) { minusIP(other); }
    void operator+(const Vector<Dim, T>& other) { add(other); }
    void operator*( double skalar ) { skalarProduct(skalar); }
public:
    Vector() = default;
    Vector(std::initializer_list<double> init)
    {
        size_t i = 0;
        for( double val : init )
        {
            if( i >= Dim ) break;
            data[i++] = val;
        }
    }
    Vector( const Vector<Dim, T>& other );
    Vector( Vector<Dim, T>&& other );
    ~Vector() = default;
private:
    std::array<double, Dim> data;
};


using Vec2D = Vector<2, double>;
using Vec3D = Vector<3, double>;
using ComplexVec3d = Vector<3, std::complex<double>>;

}
