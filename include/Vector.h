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
    T& at ( size_t i ) { return data.at(i); }
    const T& at( size_t i ) const { return data.at(i); }
    void minusIP( const Vector<Dim, T> other ) { for( int i = 0; i < Dim; i++ ) at(i) -= other[i]; }
    void fill( T value ) { data.fill(value); }
    Vector<Dim, T> minusOP( const Vector<Dim, T> other ) const 
    {
        Vector<Dim, T> res;
        for( int i = 0; i < Dim; i++ )
        {
            res[i] = at(i) - other.at(i);
        }
        return res;
    }
    void add( const Vector<Dim, T> other ) { for( int i = 0; i < Dim; i++ ) at(i) += other[i]; }
    void skalarProduct( T skalar ) { for( int i = 0; i < Dim; i++ ) at(i) *= skalar; }
    T EukNorm() const 
    { 
        T betragsquadrat = 0;
        for( int i = 0; i < Dim; i++ )
        {
            betragsquadrat += data[i] * data[i];

        }
        return sqrt(betragsquadrat); 
    }
    T& operator[]( size_t i ) { return at(i); }
    const T& operator[]( size_t i ) const { return at(i); }
    void operator-(const Vector<Dim, T>& other) { minusIP(other); }
    void operator+(const Vector<Dim, T>& other) { add(other); }
    void operator*( T skalar ) { skalarProduct(skalar); }
public:
    Vector() = default;
    Vector(std::initializer_list<T> init)
    {
        size_t i = 0;
        for( T val : init )
        {
            if( i >= Dim ) break;
            data[i++] = val;
        }
    }
    Vector( const Vector<Dim, T>& other ) : data( other.data ) {}
    Vector( Vector<Dim, T>&& other ) : data(std::move(other.data)) {}
    ~Vector() = default;
private:
    std::array<T, Dim> data;
};


using Vec2D = Vector<2, double>;
using Vec3D = Vector<3, double>;
using ComplexVec3d = Vector<3, std::complex<double>>;

}
