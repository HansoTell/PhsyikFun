#include "ShapePrinting.h"
#include <string>

namespace Physik 
{
std::string PrintShapeName(const Sphere<> sphere) { return "SPHERE"; }
std::string PrintShapeName(const Box<> box) { return "BOX"; }
    
std::string PrintShapePropertys(const Sphere<>& sphere)
{
    return std::to_string(sphere.m_Radius) += ",,,";

}
std::string PrintShapePropertys(const Box<>& box);
}
