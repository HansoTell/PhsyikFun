#include "Entity.h"
#include "System.h"
#include "Vector.h"
#include <chrono>
#include <thread>

using namespace Physik;

int main() 
{
    ClassicalSystem sys;

    ClassicEntity Sun(Vec3D{ 0.0, 0.0, 0.0 }, Vec3D{ 0.0, 0.0, 0.0 }, 1000.0);
    ClassicEntity Planet( Vec3D{ 10.0, 0.0, 0.0 }, Vec3D{ 0.0, 10.0, 0.0 }, 1.0);

    sys.addEntity(std::move(Sun));
    sys.addEntity(std::move(Planet));
    sys.addEntityPotential(CREATE_CLASSIC_ENTITY_GRAVITATIONAL_POTENTIAL());

    sys.setTimeIncrement(0.01);

    sys.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    sys.Clear();

    return 0;
}
