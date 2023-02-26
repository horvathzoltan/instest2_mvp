#ifndef BITMAPGEN_H
#define BITMAPGEN_H

#include "doublematrix.h"

#include <QColor>
#include <QMap>
#include <QSize>


class BitMapGen
{
private:
    int xMax, yMax, xMin, yMin;
    DoubleMatrix matrix;// matrix[][];
    DoubleMatrix matrixp;//double matrixp[][];
    int N, M, N_preview, M_preview;
    int intLvls = 16;
    static const int maxVer = 10;
    double rMult = 6;
    double szoras = 3;
    double R_preview = 2;
    bool preview;
    bool isShowSensor;

    QMap<int, double> mul;
    QList<QColor> rgb;

    bool isInited;
    bool _isInitializing=false;

    struct pt
    {
        int x;
        int y;
        double r;
        int ch;
        double ratio;
    };

    class pt2
    {
    public:
        pt p;
        QList<DoubleMatrix> lut;
        int n, m;
        QString name;

        pt2(){};

        pt2(pt2 const &v)
        {
            lut = v.lut;
            m = v.m;
            n = v.n;
            name = v.name;
            p = v.p;
        };
    };

    QMap<int, pt2> sensors;
    QMap<int, pt2>* Sensors() {
         return isInited ? &sensors : nullptr;
     };

    QSize Size() {
        return isInited ? QSize(N, M) : QSize();
    };

    struct EdgeBucket
    {
        int ymax;
        double xOfMin;
        double slopeInverse;
    };

    struct EdgeTableTuple
    {
        int countEdgeBucket;
        EdgeBucket buckets[];
    };

    QStringList c{
        "#3c3c3c",
        "#032787",
        "#012d95",
        "#0165ac",
        "#057ba4",
        "#0a91a1",
        "#119585",
        "#198908",
        "#819c0f",
        "#cda41f",
        "#c98626",
        "#c56623",
        "#bf2c20"
    };

    QColor _backgroundColor = QColorConstants::Black;
public:
    void Dispose();
    void Init(QString insoleMetaContent, int _prevRes, double csvMultiplier, QColor backgroundColor);
    void Init2(QString insoleMetaContent, int _prevRes, double csvMultiplier, QColor bkcolor);
    void setMaxMin(const QStringList &s);
    void fromCSV(const QStringList &s);
    void setMul(const QStringList &s, double csvMultiplier);
    void listToMap();
    void generatePt2(int key);
};

#endif // BITMAPGEN_H
