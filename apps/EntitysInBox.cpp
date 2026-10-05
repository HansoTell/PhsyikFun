#include <cstddef>
#include <random>

#include "Shapes.h"
#include "System.h"
#include "Material.h"
#include "Vector.h"

using namespace Physik;

int main()
{
    constexpr size_t NUM_ENTITYS = 5;

    size_t seed = 43627890;

    std::mt19937 generator(seed);
    std::uniform_real_distribution<> RadiusDist(0.0, 2.5);
    std::uniform_real_distribution<> Mass(1.0, 100.0);
    std::uniform_real_distribution<> Position(-50.0, 50.0);
    std::uniform_real_distribution<> Velocity( 0.0, 75.0 );

    ClassicalSystem sys;
    sys.setTimeIncrement( 0.01 );
    sys.setTmax(5000);

    //6 Box Wände Setzten
    sys.addEntity(Vec3D{ 55.0, 0.0, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ 5.0, 50.0, 50.0 } });
    sys.addEntity(Vec3D{ -55.0, 0.0, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ 5.0, 50.0, 50.0 } });
    sys.addEntity(Vec3D{ 0.0, 55.0, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ 50.0, 5.0, 50.0 }});
    sys.addEntity(Vec3D{ 0.0, -55.0, 0.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ 50.0, 5.0, 50.0 }});
    sys.addEntity(Vec3D{ 0.0, 0.0, 55.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ 50.0, 50.0, 5.0 } });
    sys.addEntity(Vec3D{ 0.0, 0.0, -55.0 } , Vec3D() , Physik::NoRotation, Vec3D() , 0.0, Material<>{ 1.0 }, Box<>{ Vec3D{ 50.0, 50.0, 5.0 } });

    for( size_t i = 0; i < NUM_ENTITYS; ++i )
        sys.addEntity(Vec3D{ Position(generator), Position(generator), Position(generator) }, Vec3D{ Velocity(generator), Velocity(generator), Velocity(generator) }, Physik::NoRotation, Vec3D(), Mass(generator), Material<>{ 1.0 }, Sphere<>{ RadiusDist(generator) } );

    sys.Start();

    const auto& f = sys.getFuture();
    f.wait();

    sys.Clear();

    return 0;
}
