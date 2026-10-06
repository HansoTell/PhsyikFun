#include "Shapes.h"
#include "System.h"
#include "Vector.h"
#include <chrono>
#include <thread>


using namespace Physik;

int main()
{
    ClassicalSystem sys;

    sys.addEntity( Vec3D{ 10.0, 0.0, 0.0 }, Vec3D{ 0.0, 1.0, 0.0 }, NoRotation, Vec3D(), 10.0, {1.0}, Sphere<>{1.0} );
    sys.addExternPotential(ClassicField( std::make_unique<ClassicStandartPotential>(100.0),  ClassicEntity(Vec3D{ 0.0, 0.0, 0.0 }, Vec3D{0.0, 0.0, 0.0}, 0.0, Sphere<>{1.0} ) ));
    sys.setTimeIncrement( 0.01 );

    sys.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    sys.Clear();
    return 0;
}


