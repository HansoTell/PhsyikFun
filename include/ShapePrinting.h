#pragma once

#include "Shapes.h"
namespace Physik 
{

std::string PrintShapeName(const Sphere<>& sphere);
std::string PrintShapeName(const Box<>& box);

std::string PrintShapePropertys(const Sphere<>& sphere);
std::string PrintShapePropertys(const Box<>& box);
    
}
