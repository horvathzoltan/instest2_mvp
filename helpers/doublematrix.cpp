#include "doublematrix.h"

DoubleMatrix::DoubleMatrix()
{
    _isInited = false;
}

void DoubleMatrix::init(int m, int n)
{
    _m = m;
    _n = n;
    _data.resize(m*n);
    _isInited = true;
}

//void DoubleMatrix::init(int m)
//{
//    _data.resize(m);
//    _isInited = true;
//}

void DoubleMatrix::setData(int m, int n, double v)
{
    int i = index(m,n);
    if(i<0) return;
    _data[i]=v;
}

double DoubleMatrix::data(int m, int n) const
{
    int i = index(m,n);
    if(i<0) return 0;
    return _data[i];
}

void DoubleMatrix::clear()
{
    _data.clear();
}

int DoubleMatrix::getLength(int i) const
{
    if(i==0) return _m;
    return _n;
}

int DoubleMatrix::index(int m, int n) const
{
    if(!_isInited) return -1;
    if(m>_m) return -1;
    if(n>_n) return -1;
    return m*_m+n;
}

