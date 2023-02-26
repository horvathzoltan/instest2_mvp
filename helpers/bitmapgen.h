#ifndef BITMAPGEN_H
#define BITMAPGEN_H

#include "doublematrix.h"

#include <QColor>
#include <QImage>
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
        QVarLengthArray<EdgeBucket> buckets;
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

    QString filename;

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
    void generateMat(int key);
    void BivariateNormalPdf(DoubleMatrix *matrix, int height, int n);
    static QColor GrayLevel(double v);
    void setFilter(DoubleMatrix *matrix, const QStringList &pts);
    void initEdgeTable(EdgeTableTuple EdgeTable[], EdgeTableTuple *ActiveEdgeTuple);
    void insertionSort(EdgeTableTuple *ett);
    void storeEdgeInTuple(EdgeTableTuple *receiver, int ym, int xm, double slopInv);
    void storeEdgeInTable(EdgeTableTuple EdgeTable[], int x1, int y1, int x2, int y2);
    void removeEdgeByYmax(EdgeTableTuple *Tup, int yy);
    void updatexbyslopeinv(EdgeTableTuple *Tup);
    bool invMat(const DoubleMatrix& mat, DoubleMatrix *inv, int a);
    double determinantOfMatrix(const DoubleMatrix &mat, int n);
    void getCofactor(const DoubleMatrix &mat, DoubleMatrix *temp, int p, int q, int n);
    void adjMat(const DoubleMatrix &mat, DoubleMatrix *adj, int a);
    DoubleMatrix getPressureMap2(const QVarLengthArray<int>& values2);
    DoubleMatrix getPressureMap3_preview(const QVarLengthArray<int>& values2);
    DoubleMatrix getPressureMap3(const QVarLengthArray<int> &values2);
    void ipolMat_preview(DoubleMatrix *matrix, QMap<int, pt2> sensors, int pressureV, int ch);
    void ipolMat(DoubleMatrix *matrix, QMap<int, pt2> sensors, int pressureV, int ch);
    QColor GetColorByLevel(double v, double limit = -1);
    static DoubleMatrix ScaleMatrix(const DoubleMatrix &pressuremap, int n);
    QImage getColoredBitmap(const DoubleMatrix &m2, double limit, bool isColored, bool isTransparent);
};

#endif // BITMAPGEN_H