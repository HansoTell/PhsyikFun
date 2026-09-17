#include "Vector.h"
#include <cmath>
#include <cstddef>

namespace Physik 
{
template<typename T = double>
class Quaternion 
{
public:
    T Re() const { return m_data[0]; }
    Vector<3, T> Im() const { return Vector<3, T>{ m_data[1], m_data[2], m_data[3] }; }
    T i() const { return m_data[1]; }
    T j() const { return m_data[2]; }
    T k() const { return m_data[3]; }
    T& real() { return m_data[0]; }
    T& i() { return m_data[1]; }
    T& j() { return m_data[2]; }
    T& k() { return m_data[3]; }

    T Norm() const { return m_data.BetragsQuadrat(); }
    T Betrag() const { return std::sqrt(Norm()); }
    T DotProduct( const Quaternion<T>& other ) const { return m_data.skalarProduct(other.m_data); }
    void NormQuaterion() { m_data/=Betrag(); }
    Quaternion<T> getEinheitsQuaternion() const 
    {
        return m_data/Betrag();
    }
    Quaternion<T> Konjugation() const { return Quaternion<T>(Re(), -i(), -j(), -k()); }
    Quaternion<T> Inverse() const { return Konjugation()/Norm(); }

    Vector<3, T> Rotate( const Vector<3, T>& PointToRotate ) const 
    {
        Quaternion<T> PointQuaternion(T{0}, PointToRotate);
        return ((*this * PointQuaternion) * this->Konjugation()).Im();
    }

    Quaternion<T>& operator+=( const Quaternion<T>& rhs ){ this->m_data += rhs.m_data; return *this; }
    Quaternion<T>& operator-=( const Quaternion<T>& rhs ){ this->m_data -= rhs.m_data; return *this; }
    Quaternion<T>& operator*=( const Quaternion<T>& rhs )
    {
        T x0 = m_data[0]*rhs.m_data[0] - m_data[1]*rhs.m_data[1] - m_data[2]*rhs.m_data[2] - m_data[3]*rhs.m_data[3];
        T x1 = m_data[0]*rhs.m_data[1] + m_data[1]*rhs.m_data[0] + m_data[2]*rhs.m_data[3] - m_data[3]*rhs.m_data[2];
        T x2 = m_data[0]*rhs.m_data[2] - m_data[1]*rhs.m_data[3] + m_data[2]*rhs.m_data[0] + m_data[3]*rhs.m_data[1];
        T x3 = m_data[0]*rhs.m_data[3] + m_data[1]*rhs.m_data[2] - m_data[2]*rhs.m_data[1] + m_data[3]*rhs.m_data[0];

        m_data[0] = x0;
        m_data[1] = x1;
        m_data[2] = x2;
        m_data[3] = x3;

        return *this;
    }
    Quaternion<T>& operator*=( const T& rhs ){ this->m_data *= rhs; return *this; }
    Quaternion<T>& operator/=( const T& scalar ) { m_data/=scalar; return *this; }

    Quaternion<T> operator+( const Quaternion<T>& rhs ) const { Quaternion<T> tmp = *this; tmp += rhs; return tmp; }
    Quaternion<T> operator-( const Quaternion<T>& rhs ) const { Quaternion<T> tmp = *this; tmp -= rhs; return tmp; }
    Quaternion<T> operator*( const Quaternion<T>& rhs ) const { Quaternion<T> tmp = *this; tmp *= rhs; return tmp; }
    Quaternion<T> operator*( const T& skalar ) const { Quaternion<T> tmp = *this; tmp *= skalar; return tmp; }
    friend Quaternion<T> operator*( const T& skalar, const Quaternion<T>& quat ) { return quat * skalar; }
    Quaternion<T> operator/( const T& skalar ) const { Quaternion<T> tmp = *this; tmp /= skalar; return tmp; }

    T operator[](size_t i) const { return m_data[i]; }
    T& operator[](size_t i) { return m_data[i]; }

    Quaternion<T>& operator=( Quaternion<T>&& other ) noexcept 
    {
        if( this == &other)
            return *this;
        m_data = std::move(other.m_data);

        return *this;
    }
    Quaternion<T>& operator=( const Quaternion<T>& other ) 
    {
        if( this == &other)
            return *this;

        m_data = other.m_data;

        return *this;
    }
public:
    Quaternion( T real, T i, T j, T k ) : m_data({ real, i, j, k} ) {}
    Quaternion( Vector<4, T> data ) : m_data(std::move(data)) {}
    Quaternion( const Quaternion&) = default;
    Quaternion(Quaternion&&) = default;
    ~Quaternion() = default;
public:
    static Quaternion FromAxisAngle( T Angle, const Vector<3, T>& Axis )
    {
        T halfAngel = Angle/T{2};
        T x0 = std::cos(halfAngel);
        Vec3D epsilon = Axis/Axis.EukNorm();
        Vec3D NormAxis = epsilon * std::sin(halfAngel);

        return Quaternion{ x0, NormAxis[0], NormAxis[1], NormAxis[2] };

    }
private:
    Vector<4, T> m_data;
};
}
