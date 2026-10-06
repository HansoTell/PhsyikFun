#include "Shapes.h"
#include "System.h"
#include "Vector.h"
#include <chrono>
#include <thread>

using namespace Physik;

int main() 
{
    ClassicalSystem sys;

    sys.addEntity(Vec3D(), Vec3D(), NoRotation, Vec3D(), 1000.0, {1.0}, Sphere<>{6.957e8});
    sys.addEntity( Vec3D{ 10.0, 0.0, 0.0 }, Vec3D{ 0.0, 10.0, 0.0 }, NoRotation, Vec3D(), 1.0, {1.0},  Sphere<>{6.37e6});
    sys.addEntityPotential(CREATE_CLASSIC_ENTITY_GRAVITATIONAL_POTENTIAL());

    sys.setTimeIncrement(0.01);

    sys.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    sys.Clear();

    return 0;
}
