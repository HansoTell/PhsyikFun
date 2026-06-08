#include "Entity.h"
#include "System.h"
#include "Vector.h"
#include <future>

using namespace Physik;

int main() 
{
    ClassicalSystem sys;

    ClassicEntity Sun(Vec3D{ 0.0, 0.0, 0.0 }, Vec3D{ 0.0, 0.0, 0.0 }, 5.683e26);
    ClassicEntity Planet1( Vec3D{ 1.2098e7, 0.0, 0.0 }, Vec3D{ 0.0, 6.9262e4, 0.0 }, 1.35e24);
    ClassicEntity Planet2( Vec3D{ 1.2342e7, 0.0, 0.0 }, Vec3D{ 0.0, 4.2158e4, 0.0 }, 1.35e24);

    sys.addEntity(std::move(Sun));
    sys.addEntity(std::move(Planet1));
    sys.addEntity(std::move(Planet2));
    sys.addEntityPotential(CREATE_CLASSIC_ENTITY_GRAVITATIONAL_POTENTIAL());

    sys.setTimeIncrement(0.5);
    sys.setTmax(5000);

    sys.Start();

    const auto& f = sys.getFuture();
    f.wait();

    sys.Clear();

    return 0;
}
