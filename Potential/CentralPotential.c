#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define beta 100.0

double g_time = 0;
double g_DeltaTime = 0.01;

void FatalError( const char* message )
{
    if( message )
    {
        printf("Fatal Error: %s \n", message);

    }else {
        printf("Fatal Error Exiting");
    }
    exit(1);
}

typedef struct 
{
    double x;
    double y;

} Vec2D_t;

typedef struct 
{
    Vec2D_t pos;
    Vec2D_t velocity;
    double mass;
} Entity_t;

void move( Entity_t* entity, double newX, double newY )
{
    entity->pos.x = newX;
    entity->pos.y = newY;
}

void printPosEntity( Entity_t* entity )
{
    printf("Pos Entity:\nX: %f \nY: %f \n", entity->pos.x, entity->pos.y );
}

double CalcBetrag( const Vec2D_t* vec )
{
    return sqrt(vec->x * vec->x + vec->y * vec->y);
}

double CalcRad( const Entity_t* entity )
{
    double posX = entity->pos.x;
    double posY = entity->pos.y;
    return sqrt( posX * posX + posY * posY );
}

Vec2D_t CalcAcceleration( const Entity_t* entity )
{
    Vec2D_t acceleration;

    double radius = CalcRad( entity );
    if( radius < 0.0001 )
        FatalError("PT ist in Kern gestürtzt");

    double Constant =  -beta / (entity->mass * radius * radius * radius);

    acceleration.x = Constant * entity->pos.x;
    acceleration.y = Constant * entity->pos.y;

    return acceleration;
}

void UpdateVelocity( Entity_t* entity, const Vec2D_t* accaleration )
{
    entity->velocity.x = entity->velocity.x + accaleration->x * g_DeltaTime;
    entity->velocity.y = entity->velocity.y + accaleration->y * g_DeltaTime;
}

void UpdatePosition ( Entity_t* entity )
{
    entity->pos.x = entity->pos.x + entity->velocity.x * g_DeltaTime;
    entity->pos.y = entity->pos.y + entity->velocity.y * g_DeltaTime;
}

double CalcEffPotential( const Entity_t* entity )
{
    double DrehImpuls = entity->mass * (entity->pos.x * entity->velocity.y - entity->pos.y * entity->velocity.x);
    double radius = CalcRad( entity );
    double Potential = - beta / radius;

    return DrehImpuls * DrehImpuls/(2 * entity->mass * radius * radius) + Potential;
}

double CalcEnergy ( const Entity_t* entity )
{
    double velocBetrag = CalcBetrag( &entity->velocity );
    double KinEnergy = entity->mass * velocBetrag * velocBetrag / 2;
    double PotentialEnergy = -beta / CalcRad(entity);

    return KinEnergy + PotentialEnergy;
}

void tick( Entity_t* entity )
{
    const Vec2D_t acceleration = CalcAcceleration( entity );
    UpdateVelocity( entity, &acceleration );
    UpdatePosition( entity );
}


int main( int agrc, char* argv[] )
{
    Entity_t entity;
    entity.pos.x = 10.0;
    entity.pos.y = 0.0;
    entity.velocity.x = 0.0;
    entity.velocity.y = 1.0;
    entity.mass = 10.0;

    printf("Energy ges: %f J\n", CalcEnergy( &entity ));
    while ( g_time < 10 )
    {
        tick( &entity );
        printf("Effective Potential: %f \n", CalcEffPotential( &entity ));
        printPosEntity( &entity );
        g_time += g_DeltaTime;
    }

    return 0;
}
