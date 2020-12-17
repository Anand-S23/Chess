#ifndef MATH_H
#define MATH_H

#define Bytes(n)     (n)
#define Kilobytes(n) (Bytes(n)*1024LL)
#define Megabytes(n) (Kilobytes(n)*1024LL)
#define Gigabytes(n) (Megabytes(n)*1024LL)
#define Terabytes(n) (Gigabytes(n)*1024LL)

#define PIf 3.1415926535897f
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PI 0.3989422804
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PIf 0.3989422804f
#define EULERS_NUMBER 2.7182818284590452353602874713527
#define EULERS_NUMBERf 2.7182818284590452353602874713527f

#define ArraySize(arr) (sizeof(arr) / sizeof((arr)[0]))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define AbsoluteValue(a) ((a) > 0 ? (a) : -(a))

typedef union v2 
{
    struct
    {
        float x; 
        float y; 
    }; 

    struct 
    {
        float width; 
        float height;
    };
    
} v2;

typedef union v3 
{
    struct 
    {
        float x;
        float y;
        float z;
    };
    
    struct 
    {
        float r;
        float g;
        float b;
    };
    
    float elements[3];
} v3;

typedef union v4 
{
    struct 
    {
        float x;
        float y;
        union 
        {
            struct 
            {
                float z;
                union
                {
                    float w;
                    float radius;
                };
            };

            struct 
            {
                float width;
                float height;
            };
        };
    };
    
    struct 
    {
        float r;
        float g;
        float b;
        float a;
    };
    
    float elements[4];
} v4;

internal v2 V2Init(float x, float y)
{
    v2 v = { x, y };
    return v;
}
#define v2(x, y) V2Init(x, y)

internal v3 V3Init(float x, float y, float z)
{
    v3 v = { x, y, z };
    return v; 
}
#define v3(x, y, z) V3Init(x, y, z)

internal v4 V4Init(float x, float y, float z, float w)
{
    v4 v = { x, y, z, w }; 
    return v; 
}
#define v4(x, y, z, w) V4Init(x, y, z, w)

#endif