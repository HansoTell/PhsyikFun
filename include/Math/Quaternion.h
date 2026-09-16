#include "Vector.h"
#include <cmath>
namespace Physik 
{
//-> Konstrukto: (x0: Eingabe Winkel alpha (zwischen 0, 2pi)-> x0 = cos(alpha/2), x1-x3: Eingabe Achsen -> zu normierten Achsen transformieren.  ) -> wie von normalen unterscheiden?
///-> wie unterschiede von normalen Quaternionen?
template<typename T = double>
class Quaternion 
{
public:
    T real() const { return m_data[0]; }
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
    Quaternion<T> Konjugation(){ return Quaternion<T>(real(), -i(), -j(), -k()); }

    Quaternion<T>& operator+=( const Quaternion<T>& rhs ){ *this += rhs; return *this; }
    Quaternion<T>& operator-=( const Quaternion<T>& rhs ){ *this -= rhs; return *this; }

public:
    Quaternion( T Drehwinkel, Vec3D Axis ) 
    {
        T alpha = Drehwinkel/2;
        T x0 = std::cos(alpha);
        Vec3D epsilon = Axis/Axis.EukNorm();
        Vec3D NormAxis = epsilon * std::sin(alpha);
        m_data = { x0, NormAxis[0], NormAxis[1], NormAxis[2] };
    }
    explicit Quaternion( T real, T i, T j, T k ) : m_data({ real, i, j, k} ) {}
    Quaternion( Vector<4, T> data ) : m_data(std::move(data)) {}
    Quaternion( const Quaternion&) = default;
    Quaternion(Quaternion&&) = default;
    ~Quaternion() = default;
private:
    Vector<4, T> m_data;
};


}
