#pragma once

#include <vector>
#include <string>


namespace Physik 
{
//idee methoden einsetztenw ei set dimensions die beim vekltor direkt einsetzten mit welchen dimensionen man arbeitet genau so mit welcher zahlen menge man arbeitet

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

    double m_Time;
    double m_DelatTime;
};

class RelativistikSystem : public ISystem 
{

};
}
