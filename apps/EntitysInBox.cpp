#include <cassert>
#include <cfenv>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <fenv.h>
#include <memory>
#include <random>

#include "Shapes.h"
#include "System.h"
#include "Material.h"
#include "Vector.h"

using namespace Physik;

#define DYNMAIC_BOX_SIZE 1 

int main(int argc, char* argv[])
{
    //feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW);
    assert(argc == 2);
    int numEntitys = std::atoi(argv[1]);
    assert(numEntitys != 0);

    //constexpr size_t NUM_ENTITYS = 5;

    size_t seed = 43627890;

    std::mt19937 generator(seed);

    constexpr double RMAX = 2.5;
    const double Height = DYNMAIC_BOX_SIZE ? 50.0 * std::cbrt(numEntitys/100.0) : 50.0;
    const double depth = 0.5 * Height;
    const double MiddlePoint = Height+depth;
    const double Out = Height + 2.0 * depth;

    std::uniform_real_distribution<> RadiusDist(0.0, RMAX);
    std::uniform_real_distribution<> Mass(1.0, 100.0);
    std::uniform_real_distribution<> Position(-(Height - RMAX), Height-RMAX);
    std::uniform_real_distribution<> Velocity( 0.0, 25.0 );

    ClassicalSystem sys(std::make_unique<FakePrinter>());
    sys.setTimeIncrement( 0.01 );
    sys.setTmax(100);

    //6 Box Wände Setzten
    sys.addEntity({ Vec3D{ MiddlePoint, 0.0, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ depth, Height, Height} } });
    sys.addEntity({ Vec3D{ -MiddlePoint, 0.0, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ depth, Height, Height} }});
    sys.addEntity({ Vec3D{ 0.0, MiddlePoint, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ Out, depth, Height}}});
    sys.addEntity({ Vec3D{ 0.0, -MiddlePoint, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ Out, depth, Height}}});
    sys.addEntity({ Vec3D{ 0.0, 0.0, MiddlePoint} , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ Out, Out, depth} }});
    sys.addEntity({ Vec3D{ 0.0, 0.0, -MiddlePoint} , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ Out, Out, depth} }});

    for( int i = 0; i < numEntitys; ++i )
        sys.addEntity({ Vec3D{ Position(generator), Position(generator), Position(generator) }, Vec3D{ Velocity(generator), Velocity(generator), Velocity(generator) }, Physik::NoRotation, Vec3D(), Mass(generator), Material<>{ 1.0 }, Sphere<>{ RadiusDist(generator) } });

    sys.Start();

    const auto& f = sys.getFuture();
    f.wait();

    sys.Clear();

    return 0;
}
