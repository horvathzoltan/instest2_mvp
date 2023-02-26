#ifndef DOUBLEMATRIX_H
#define DOUBLEMATRIX_H

#include <QVarLengthArray>



class DoubleMatrix
{
private:
    QVarLengthArray<double> _data;
    int _m;
    int _n;
    bool _isInited;
public:
    DoubleMatrix();
    void init(int m, int n);
    void setData(int m, int n, double v);
    double data(int m, int n);
    void clear();
private:
    int index(int m, int n);
};

#endif // DOUBLEMATRIX_H
