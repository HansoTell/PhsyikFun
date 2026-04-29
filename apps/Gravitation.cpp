#include "Entity.h"
#include "Interactions.h"
#include "System.h"
#include "Vector.h"
#include <chrono>
#include <memory>
#include <thread>

using namespace Physik;

int main() 
{
    ClassicalSystem sys;

    ClassicEntity mercury(CREATE_POSITON_VEC3D(0.0, 0.0, 0.0), Vec3D{1.0, 0.0, 0.0}, 1000.0);

    ClassicEntity venus(CREATE_POSITON_VEC3D(10.0, 0.0, 0.0), Vec3D{0.0, 10.0, 0.0}, 1.0);

    sys.addEntity(std::move(mercury));
    sys.addEntity(std::move(venus));
    sys.addEntityPotential( ClassicInteraction( std::make_unique<ClassicGravitationPotential>()) );

    sys.setTimeIncrement(0.01);

    sys.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    sys.Clear();

    /*
    double Constant = 100.0;
    mercury.addIntrinsivPotential(PotentialTypes::STANDART_POTENTIAL, (void*)&Constant);
    */
    return 0;
}
