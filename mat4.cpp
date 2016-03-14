#include <stdio.h>
#include <stdlib.h>
#include <utility>

#include "./linear_algebra.hpp"

double& mat4::operator[](size_t arg)
{
    if (arg < 16)
    {
        return mat[arg];
    }
    else
    {
        fprintf(stderr, "%s : セグメンテーションエラー\n", __func__);
        exit(EXIT_FAILURE);
    }
}
mat4& mat4::operator=(const mat4 op)
{
    *this = std::move(op);
    this->determinant_();
    this->inverse_();
    for (unsigned int i = 0; i < 16; i++)
        fltbuf[i] = mat[i];
    return *this;
}
mat4 mat4::operator*(const mat4 op)
{
    mat4 ret(mat4::O);
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            for (unsigned int k = 0; k < 4; k++)
            {
                ret.mat[i*4+j] += this->mat[i*4+k] * op.mat[k*4+j];
            }
        }
    }
    return ret;
}
mat4& mat4::operator*=(const mat4 op)
{
    return *this = *this * op;
}
mat4 mat4::operator+(const mat4 op)
{
    mat4 ret(mat4::O);
    for (unsigned int i = 0; i < 16; i++) {
        ret.mat[i] = this->mat[i] + op.mat[i];
    }
    return ret;
}
mat4& mat4::operator+=(const mat4 op)
{
    return *this = *this + op;
}
mat4 mat4::operator-(const mat4 op)
{
    mat4 ret(mat4::O);
    for (unsigned int i = 0; i < 16; i++) {
        ret.mat[i] = this->mat[i] - op.mat[i];
    }
    return ret;
}
mat4& mat4::operator-=(const mat4 op)
{
    return *this = *this - op;
}
vec4 mat4::operator*(const vec4 b)
{
    vec4 ret = {};
    ret.x = 
        mat[0]  * b.x +
        mat[4]  * b.y +
        mat[8]  * b.z +
        mat[12] * b.w ;
    ret.y =
        mat[1]  * b.x +
        mat[5]  * b.y +
        mat[9]  * b.z +
        mat[13] * b.w ;
    ret.z =
        mat[2]  * b.x +
        mat[6]  * b.y +
        mat[10] * b.z +
        mat[14] * b.w ;
    ret.w =
        mat[3]  * b.x +
        mat[7]  * b.y +
        mat[11] * b.z +
        mat[15] * b.w ;
    return ret;
}
float * mat4::getFloat()
{
    return this->fltbuf;
}
double * mat4::getDouble()
{
    return this->mat;
}
void mat4::determinant_()
{
    double temp1 = mat[0] * (mat[ 5]*mat[10]*mat[15] + mat[ 7]*mat[ 9]*mat[14] + mat[ 6]*mat[11]*mat[13]  
                           - mat[ 7]*mat[10]*mat[13] - mat[11]*mat[14]*mat[ 5] - mat[15]*mat[ 6]*mat[ 9]);
    double temp2 = mat[1] * (mat[ 4]*mat[10]*mat[15] + mat[ 7]*mat[ 8]*mat[14] + mat[ 6]*mat[11]*mat[12]
                           - mat[ 4]*mat[11]*mat[14] - mat[ 7]*mat[10]*mat[12] - mat[15]*mat[ 6]*mat[ 8]);
    double temp3 = mat[2] * (mat[ 4]*mat[ 9]*mat[15] + mat[ 5]*mat[11]*mat[12] + mat[ 7]*mat[ 8]*mat[13]
                           - mat[ 4]*mat[11]*mat[13] - mat[ 7]*mat[ 9]*mat[12] - mat[15]*mat[ 5]*mat[ 8]);
    double temp4 = mat[3] * (mat[ 4]*mat[ 9]*mat[14] + mat[ 5]*mat[10]*mat[12] + mat[ 6]*mat[ 8]*mat[13]
                           - mat[ 4]*mat[10]*mat[13] - mat[ 5]*mat[ 8]*mat[14] - mat[ 6]*mat[ 9]*mat[12]);

    this->det_ = temp1 - temp2 + temp3 - temp4;
}
void mat4::inverse_(void)//priv
{
    double tempmat[16] = {};

    tempmat[ 0] =  (mat[ 5]*mat[10]*mat[15] + mat[ 7]*mat[ 9]*mat[14] + mat[ 6]*mat[11]*mat[13]  
                  - mat[ 7]*mat[10]*mat[13] - mat[11]*mat[14]*mat[ 5] - mat[15]*mat[ 6]*mat[ 9])/ this->det_;
    tempmat[ 4] = -(mat[ 4]*mat[10]*mat[15] + mat[ 7]*mat[ 8]*mat[14] + mat[ 6]*mat[11]*mat[12]
                  - mat[ 4]*mat[11]*mat[14] - mat[ 7]*mat[10]*mat[12] - mat[15]*mat[ 6]*mat[ 8]) / this->det_;
    tempmat[ 8] =  (mat[ 4]*mat[ 9]*mat[15] + mat[ 5]*mat[11]*mat[12] + mat[ 7]*mat[ 8]*mat[13]
                  - mat[ 4]*mat[11]*mat[13] - mat[ 7]*mat[ 9]*mat[12] - mat[15]*mat[ 5]*mat[ 8]) / this->det_;
    tempmat[12] = -(mat[ 4]*mat[ 9]*mat[14] + mat[ 5]*mat[10]*mat[12] + mat[ 6]*mat[ 8]*mat[13]
                  - mat[ 4]*mat[10]*mat[13] - mat[ 5]*mat[ 8]*mat[14] - mat[ 6]*mat[ 9]*mat[12]) / this->det_;
    tempmat[ 1] = -(mat[ 1]*mat[10]*mat[15] + mat[ 9]*mat[14]*mat[ 3] + mat[13]*mat[ 2]*mat[11]
                  - mat[ 3]*mat[10]*mat[13] - mat[11]*mat[14]*mat[ 1] - mat[15]*mat[ 2]*mat[ 9]) / this->det_;
    tempmat[ 5] =  (mat[ 0]*mat[10]*mat[15] + mat[ 8]*mat[14]*mat[ 3] + mat[12]*mat[ 2]*mat[11]
                  - mat[ 3]*mat[10]*mat[12] - mat[11]*mat[14]*mat[ 0] - mat[15]*mat[ 2]*mat[ 8]) / this->det_;
    tempmat[ 9] = -(mat[ 0]*mat[ 9]*mat[15] + mat[ 8]*mat[13]*mat[ 3] + mat[12]*mat[ 1]*mat[11]
                  - mat[ 3]*mat[ 9]*mat[12] - mat[11]*mat[13]*mat[ 0] - mat[15]*mat[ 1]*mat[ 8]) / this->det_;
    tempmat[13] =  (mat[ 0]*mat[ 9]*mat[14] + mat[ 8]*mat[13]*mat[ 2] + mat[12]*mat[ 1]*mat[10]
                  - mat[ 2]*mat[ 9]*mat[12] - mat[10]*mat[13]*mat[ 0] - mat[14]*mat[ 1]*mat[ 8]) / this->det_;
    tempmat[ 2] =  (mat[ 1]*mat[ 6]*mat[15] + mat[ 5]*mat[14]*mat[ 3] + mat[13]*mat[ 2]*mat[ 7]
                  - mat[ 3]*mat[ 6]*mat[13] - mat[ 7]*mat[14]*mat[ 1] - mat[15]*mat[ 2]*mat[ 5]) / this->det_;
    tempmat[ 6] = -(mat[ 0]*mat[ 6]*mat[15] + mat[ 4]*mat[14]*mat[ 3] + mat[12]*mat[ 2]*mat[ 7]
                  - mat[ 3]*mat[ 6]*mat[12] - mat[ 7]*mat[14]*mat[ 0] - mat[15]*mat[ 2]*mat[ 4]) / this->det_;
    tempmat[10] =  (mat[ 0]*mat[ 5]*mat[15] + mat[ 4]*mat[13]*mat[ 3] + mat[12]*mat[ 1]*mat[ 7]
                  - mat[ 3]*mat[ 5]*mat[12] - mat[ 7]*mat[13]*mat[ 0] - mat[15]*mat[ 1]*mat[ 4]) / this->det_;
    tempmat[14] = -(mat[ 0]*mat[ 5]*mat[14] + mat[ 4]*mat[13]*mat[ 2] + mat[12]*mat[ 1]*mat[ 6]
                  - mat[ 2]*mat[ 5]*mat[12] - mat[ 6]*mat[13]*mat[ 0] - mat[14]*mat[ 1]*mat[ 4]) / this->det_;
    tempmat[ 3] = -(mat[ 1]*mat[ 6]*mat[11] + mat[ 5]*mat[10]*mat[ 3] + mat[ 9]*mat[ 2]*mat[ 7]
                 -  mat[ 3]*mat[ 6]*mat[ 9] - mat[ 7]*mat[10]*mat[ 1] - mat[11]*mat[ 2]*mat[ 5]) / this->det_;
    tempmat[ 7] =  (mat[ 0]*mat[ 6]*mat[11] + mat[ 4]*mat[10]*mat[ 3] + mat[ 8]*mat[ 2]*mat[ 7]
                  - mat[ 3]*mat[ 6]*mat[ 8] - mat[ 7]*mat[10]*mat[ 0] - mat[11]*mat[ 2]*mat[ 4]) / this->det_;
    tempmat[11] = -(mat[ 0]*mat[ 5]*mat[11] + mat[ 4]*mat[ 9]*mat[ 3] + mat[ 8]*mat[ 1]*mat[ 7]
                  - mat[ 3]*mat[ 5]*mat[ 8] - mat[ 7]*mat[ 9]*mat[ 0] - mat[11]*mat[ 1]*mat[ 4]) / this->det_;
    tempmat[15] =  (mat[ 0]*mat[ 5]*mat[10] + mat[ 4]*mat[ 9]*mat[ 2] + mat[ 8]*mat[ 1]*mat[ 6]
                  - mat[ 2]*mat[ 5]*mat[ 8] - mat[ 6]*mat[ 9]*mat[ 0] - mat[10]*mat[ 1]*mat[ 4]) / this->det_;
    for (unsigned int i = 0; i < 16; i++)
    {
        this->inverseMatrix[i] = (float)tempmat[i];
    }
}
double mat4::detd(void)
{
    return this->det_;
}
float mat4::detf(void)
{
    return static_cast<float>(this->det_);
}
float * mat4::inverse(void)
{
    return this->inverseMatrix;
}
const float * mat4::inverse(void) const
{
    return this->inverseMatrix;
}
void mat4::update()
{
    this->determinant_();
    this->inverse_();
    for (unsigned int i = 0; i < 16; i++)
        fltbuf[i] = mat[i];
}
mat4::mat4(enumMat4 mattype) : mat(), fltbuf(), det_(), inverseMatrix()
{
    if (mattype == I)
    {
        mat[0] = mat[5] = mat[10] = mat[15] = 1.0;
        fltbuf[0] = fltbuf[5] = fltbuf[10] = fltbuf[15] = 1.0;
    }
}
