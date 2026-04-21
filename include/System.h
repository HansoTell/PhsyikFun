#pragma once

#include "Entity.h"
#include "Vector.h"
#include "Printer.h"
#include "Potential.h"
#include <memory>
#include <vector>


namespace Physik 
{
//idee methoden einsetztenw ei set dimensions die beim vekltor direkt einsetzten mit welchen dimensionen man arbeitet genau so mit welcher zahlen menge man arbeitet

class ISystem 
{
public: 
    ~ISystem() = default;
    virtual void Run() = 0;
    virtual void Pause() = 0;
    virtual void Clear() = 0;
};

class ClassicalSystem : public ISystem 
{
public:
    //wie interactiv machen mit dimensionen??
    using Vector = Vector<3, double>;

    void Run() override;
    void Pause() override;
    void Clear() override;
    void addPotential( std::unique_ptr<ClassicPotential> potential );
    void addMulitpPotentials( std::vector<std::unique_ptr<ClassicPotential>> potentials );
    void addEntity( ClassicEntity entity );
    void addMulipleEntitys( std::vector<ClassicEntity> entitys );
    void setTimeIncrement( double DeltaTime );

public:
    ClassicalSystem();
    ClassicalSystem( std::unique_ptr<IPrinter> printer );
    ClassicalSystem(const ClassicalSystem& other);
    ClassicalSystem(ClassicalSystem&& other) = delete;
    ~ClassicalSystem() = default;
private:
    std::vector<ClassicEntity> m_Entitys;
    std::vector<std::unique_ptr<ClassicPotential>> m_Potentials;

    double m_Time;
    double m_DelatTime;
    //??
    double Energy;

    bool m_running;
    std::unique_ptr<IPrinter> m_Printer;
};

class RelativistikSystem : public ISystem 
{

};
}
