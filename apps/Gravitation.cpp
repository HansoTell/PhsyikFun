#include "Entity.h"
#include "System.h"

using namespace Physik;

int main() 
{
    ClassicalSystem sys;

    sys.addPotential(CREATE_CLASSIC_GRAVITATIONAL_POTENTIAL(1.989e30, 0.0, 0.0, 0.0));

    ClassicEntity mercury(CREATE_POSITON_VEC3D(10.0, 0.0, 0.0), 3.285e23);

}
