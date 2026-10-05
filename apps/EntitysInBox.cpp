#include <cstddef>
#include <random>

#include "System.h"

using namespace Physik;

int main()
{
    constexpr size_t NUM_ENTITYS = 50;


    size_t seed = 43627890;

    std::mt19937 generator(seed);

    std::uniform_real_distribution<> distrobution(1.0, 2.0);

    double t = distrobution(generator);

    ClassicalSystem sys;

    sys.addEntity();

    for( size_t i = 0; i < NUM_ENTITYS; ++i )
    {

    }


    



    return 0;
}
