#include "Shapes.h"
#include "System.h"
#include "Vector.h"
#include <future>

using namespace Physik;

int main() 
{
    ClassicalSystem sys;

    sys.addEntity(Vec3D(), Vec3D(), NoRotation, Vec3D(), 5.683e26, {1.0}, Sphere<>{6.957e8});
    sys.addEntity( Vec3D{ 1.2098e7, 0.0, 0.0 }, Vec3D{ 0.0, 6.9262e4, 0.0 }, NoRotation, Vec3D(), 1.35e24, {1.0}, Sphere<>{6.37e6});
    sys.addEntity( Vec3D{ 1.2342e7, 0.0, 0.0 }, Vec3D{ 0.0, 4.2158e4, 0.0 }, NoRotation, Vec3D(), 1.35e24, {1.0}, Sphere<>{1.738e6});
    sys.addEntityPotential(CREATE_CLASSIC_ENTITY_GRAVITATIONAL_POTENTIAL());

    sys.setTimeIncrement(0.5);
    sys.setTmax(5000);

    sys.Start();

    const auto& f = sys.getFuture();
    f.wait();

    sys.Clear();

    return 0;
}
