#pragma once

#include "Vector.h"
#include "Potential.h"
#include <memory>
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
    //wie interactiv machen mit dimensionen??
    using Vector = Vector<3, double>;

    void addPotential( std::unique_ptr<ClassicPotential> potential );
    void addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials );
    void addEntity();
    void addMulipleEntitys();
    void setTimeIncrement( double DeltaTime );

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
    //??
    double Energy;
};

class RelativistikSystem : public ISystem 
{

};
}
