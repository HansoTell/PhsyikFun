#pragma once

#include "Vector.h"
#include "Math/Quaternion.h"
#include "Material.h"
#include "Shapes.h"

namespace Physik 
{
struct EntityDescription
{ 
    Vec3D Position;
    Vec3D Velocity;
    Quaternion<> Rotation;
    Vec3D AngularVelocity;
    double Mass;
    Material<> material; 
    Shape<> shape; 
};
    
}
