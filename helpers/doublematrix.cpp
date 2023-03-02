#include "doublematrix.h"

DoubleMatrix::DoubleMatrix()
{
    _isInited = false;
}

DoubleMatrix::DoubleMatrix(const DoubleMatrix& a){
    _n = a._n;
    _m = a._m;
    _isInited = a._isInited;
    _data = QVarLengthArray<double>(a._data);
}

void DoubleMatrix::init(int n, int m)
{
    _n = n;
    _m = m;
    _data.resize(m*n);
    _isInited = true;
}

//void DoubleMatrix::init(int m)
//{
//    _data.resize(m);
//    _isInited = true;
//}

bool DoubleMatrix::setData(int n, int m, double v)
{
    int i = index(n,m);
    if(i<0) return false;
    _data[i]=v;
    return true;
}

bool DoubleMatrix::data(int n, int m, double* d) const
{
    if(!d) return false;
    int i = index(n,m);
    if(i<0) return false;
    *d=_data[i];
    return true;
}

void DoubleMatrix::clear()
{
    _data.clear();
}

int DoubleMatrix::getLength(int i) const
{
    if(i==0) return _n;
    return _m;
}

int DoubleMatrix::index(int n, int m) const
{
    if(!_isInited) return -1;
    if(m>=_m) return -1;
    if(n>=_n) return -1;
    //return n*_m+m;
    return m*_n+n;
}

