#include "Entity.h"
#include "System.h"
#include "Vector.h"
#include <chrono>
#include <thread>


using namespace Physik;

int main()
{
    ClassicalSystem sys;
    ClassicEntity ent( CREATE_POSITON_VEC3D(10.0, 0.0, 0.0), Vec3D{ 0.0, 1.0, 0.0 }, 10.0 );
    sys.addEntity(std::move(ent));
    sys.addExternPotential(ClassicField( std::make_unique<ClassicStandartPotential>(100.0), { CREATE_POSITON_VEC3D(0.0, 0.0, 0.0), Vec3D{0.0, 0.0, 0.0}, 0.0 } ));
    sys.setTimeIncrement( 0.01 );

    sys.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    sys.Clear();
    return 0;
}


