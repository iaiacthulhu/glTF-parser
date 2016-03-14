/*

*/
#ifndef LINEAR_ALGEBRA_HPP
#define LINEAR_ALGEBRA_HPP

typedef struct vec4_t
{
    union
    {
        struct
        {
            double x;
            double y;
            double z;
            double w;
        };
        struct
        {
            double r;
            double g;
            double b;
            double a;
        };
        struct
        {
            double s;
            double t;
            double p;
            double q;
        };
    };
}vec4;

typedef struct vec3_t
{
    union
    {
        struct
        {
            double x;
            double y;
            double z;
        };
        struct
        {
            double r;
            double g;
            double b;
        };
        struct
        {
            double s;
            double t;
            double p;
        };
    };
}vec3;

typedef struct vec2_t
{
    union
    {
        struct
        {
            double x;
            double y;
        };
        struct
        {
            double u;
            double v;
        };
        struct
        {
            double s;
            double t;
        };
    };
}vec2;

class mat4
{
    double mat[16];
    float fltbuf[16];
    double det_;
    float inverseMatrix[16];
    public:
    enum enumMat4
    {
        O = 0, // zero matrix
        I = 1, // identity matrix
    };
    double& operator[](size_t);
    mat4& operator = (const mat4);
    mat4  operator * (const mat4);
    mat4& operator *=(const mat4);
    mat4  operator + (const mat4);
    mat4& operator +=(const mat4);
    mat4  operator - (const mat4);
    mat4& operator -=(const mat4);
    vec4  operator * (const vec4);
    float * getFloat();
    double * getDouble();
    private:
    void determinant_(void);
    void inverse_(void);
    public:
    double detd(void);
    float detf(void);
    float * inverse(void);
    const float * inverse(void) const;
    void update();

    mat4(enumMat4 mattype = I);
};

#endif
