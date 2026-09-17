#include "Entity.h"
#include "Interactions.h"
#include "Shapes.h"
#include "System.h"
#include "Vector.h"
#include <future>

using namespace Physik;

int main()
{
    ClassicalSystem sys;

    ClassicEntity Sun(Vec3D{ 0.0, 0.0, 0.0 }, Vec3D{ 100.0, 0.0, 0.0 }, 1.0, Sphere<>{1.0});

    sys.addEntity(std::move(Sun));
    sys.addNonPotentialForce(ClassicNonPotentialForce([]( const ClassicEntity& state, double Time ){ 
        constexpr double gamma = 0.01; 

        return -gamma * state.getVelocity(); 
    }));

    sys.setTimeIncrement(0.1);
    sys.setTmax(5000);

    sys.Start();

    const auto& f = sys.getFuture();
    f.wait();

    sys.Clear();

    return 0;
}
