#pragma once

#include <vector>
#include <string>


namespace Physik 
{

class ISystem 
{
public: 
    ~ISystem() = default;

};

class ClassicalSystem : public ISystem 
{
public:

public:
    ClassicalSystem();
    ClassicalSystem(const ClassicalSystem& other);
    ClassicalSystem(ClassicalSystem&& other) = delete;
    ~ClassicalSystem() = default;
private:
    std::vector<std::string> m_Entitys;
    std::vector<std::string> m_Potentials;
};

class RelativistikSystem : public ISystem 
{

}
}
