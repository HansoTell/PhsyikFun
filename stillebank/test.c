#include <math.h>
#include <stddef.h>
#include <stdlib.h>

typedef float (*Func1D) (float, void*);

typedef struct 
{
    float* ai;
    size_t num;
} PolynomialParams_t;

typedef struct 
{
    Func1D func;
    void* params;
} Function_t;

float CalcPolynomial( float x, void* params)
{
    PolynomialParams_t* param = (PolynomialParams_t*) params;
    float erg = 0.0f;
    for( int i = 0; i < param->num; i++ )
    {
        erg += param->ai[i] * pow(x, i);
    }

    return erg;
}

float Integ1dApprox(const Function_t* f, float x)
{
    f->func(x, f->params);

    return 0;
}

int main(int argc, char* argv[])
{

    PolynomialParams_t params2;
    params2.num = 4;
    params2.ai = (float*) malloc(4 * sizeof(float));
    params2.ai[0] = params2.ai[2] = 0;
    params2.ai[1] = -0.5f;
    params2.ai[3] = 1.0f;

    Function_t bspFunc;
    bspFunc.func = CalcPolynomial;
    bspFunc.params = (void*) &params2;

    float erg = Integ1dApprox(&bspFunc, 10.0f);
}
