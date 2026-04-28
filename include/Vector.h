#pragma once

#include <array>
#include <cmath>
#include <complex>
#include <cstddef>
#include <initializer_list>
#include <ostream>
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
    void fill( T value ) { data.fill(value); }
    void skalarProduct( T skalar ) { for( int i = 0; i < Dim; i++ ) at(i) *= skalar; }
    T EukNorm() const 
    { 
        T betragsquadrat = 0;
        for( int i = 0; i < Dim; i++ )
        {
            betragsquadrat += data[i] * data[i];

        }
        return std::sqrt(betragsquadrat); 
    }
    Vector<Dim, T>& operator=( Vector<Dim, T>&& other ) noexcept 
    {
        if( this == &other)
            return *this;

        data = std::move(other.data);

        return *this;
    } 
    Vector<Dim, T>& operator=( const Vector<Dim, T>& other ) 
    {
        if( this == &other)
            return *this;

        data = other.data;

        return *this;
    }
    T& operator[]( size_t i ) { return at(i); }
    const T& operator[]( size_t i ) const { return at(i); }
    Vector<Dim, T>& operator+=(const Vector<Dim, T>& rhs)
    { 
        for( size_t i = 0; i < rhs.data.size(); i++ )
            data[i] += rhs[i];
        return *this;
    }
    Vector<Dim, T>& operator-=(const Vector<Dim, T>& rhs)
    { 
        for( size_t i = 0; i < rhs.data.size(); i++ )
            data[i] -= rhs[i];
        return *this;
    }
    Vector<Dim, T>& operator*=( T scalar ) 
    {
        for( size_t i = 0; i < data.size(); i++ )
            data[i] *= scalar;

        return *this;
    }
    Vector<Dim, T> operator+(const Vector<Dim, T>& other) const 
    { 
        Vector<Dim, T> tmp = *this;
        tmp+=other;
        return tmp; 
    }
    Vector<Dim, T> operator-(const Vector<Dim, T>& other) const 
    { 
        Vector<Dim, T> tmp = *this;
        tmp -= other;
        return tmp;
    }
    Vector<Dim, T> operator*( T skalar ) 
    { 
        Vector<Dim, T> tmp = *this;
        tmp *= skalar;
        return tmp;
    }
    friend Vector<Dim, T> operator*(T scalar, const Vector<Dim, T>& vec)
    {
        return vec * scalar;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vector<Dim, T>& vec)
    {
        os << "(";

        for( int i = 0; i < Dim; i++ )
        {
            os << vec[i];
            if( i+1 < Dim ) 
                os << ", ";
        }


        os << ")";
        return os;
    }
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
