#include "Entity.h"
#include "Potential.h"
#include "System.h"
#include "Vector.h"
#include <memory>

using namespace Physik;

int main()
{
    ClassicalSystem sys;
    std::unique_ptr<ClassicPotential> potential = std::make_unique<ClassicStandartPotential>(100.0, CREATE_POSITON_VEC3D(0.0, 0.0, 0.0) );
    ClassicEntity ent( CREATE_POSITON_VEC3D(10.0, 0.0, 0.0), 10.0, Vec3D{ 0.0, 1.0, 0.0 } );

    sys.addEntity(std::move(ent));
    sys.addPotential(std::move(potential));
    sys.setTimeIncrement( 0.001 );

    sys.Run();

    return 0;
}


