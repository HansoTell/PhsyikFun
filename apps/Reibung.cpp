#include "Entity.h"
#include "Interactions.h"
#include "System.h"
#include "Vector.h"
#include <future>

using namespace Physik;

int main()
{
    ClassicalSystem sys;

    ClassicEntity Sun(Vec3D{ 0.0, 0.0, 0.0 }, Vec3D{ 100.0, 0.0, 0.0 }, 1);

    sys.addEntity(std::move(Sun));
    sys.addNonPotentialForce(ClassicNonPotentialForce([]( const ClassicEntityState& state, double Time ){ 
        constexpr double gamma = 0.01; 

        return -gamma * state.m_KinState.m_Velocity; 
    }));

    sys.setTimeIncrement(0.1);
    sys.setTmax(5000);

    sys.Start();

    const auto& f = sys.getFuture();
    f.wait();

    sys.Clear();

    return 0;
}
