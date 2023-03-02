#include "bitmapgen.h"
#include "logger.h"

#include <QtMath>
#include <QImage>

QList<QColor> BitMapGen::_rgb;

QStringList BitMapGen::_rgbTxt{
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

void BitMapGen::Dispose()
{
    if (_isInitializing) return;// igazán izgalmas a szituáció
    zInfo("BitmapGen_Dispose ****************");

    mul.clear();
    //mul = null;
    sensors.clear();
    //sensors = null;

    matrix.clear();
    matrixp.clear();
}

BitMapGen::BitMapGen()
{
    if(_rgb.empty()){
        BitMapGen::initRgb();
    }
}

void BitMapGen::setPreview(bool v)
{
    _preview=v;
}

void BitMapGen::initRgb(){
    if(_rgbTxt.isEmpty()){
        zInfo("cannot initRgb: _rgbTxt is empty");
        return;
    }
    //QList<QColor> rgb;
    for(auto& item:_rgbTxt)
    {
        QColor x(item);
        _rgb.append(x);
    }
    //return rgb;
}

void BitMapGen::Init(QString insoleMetaContent, int _prevRes,
          double csvMultiplier)
{
    if (_isInitializing) return;
    _isInitializing = true;

    Init2(insoleMetaContent, _prevRes, csvMultiplier);
    _isInitializing = false;
}

void BitMapGen::Init2(QString insoleMetaContent,
                      int _prevRes, double csvMultiplier)
{
    isInited = false;
    if (insoleMetaContent.isEmpty()) return;
    R_preview = _prevRes;
    //insolename = insolename.zNormalize();
    //_backgroundColor = bkcolor;

    auto lines = insoleMetaContent.split('\n');   

    xMax = yMax = std::numeric_limits<int>::min();
    xMin = yMin = std::numeric_limits<int>::max();

    setMaxMin(lines);
    //FootprintSideEnum.Left

    ///// fill sensors
    sensors = QMap<int, pt2>();
    fromCSV(lines);
    if (sensors.count() == 0)
    {
        _lasterr = QStringLiteral("NOINSOLEDATAINFILE");
        return;
    }
    /// ha itt jön egy dispose
    listToMap();
    ///////////
    N = xMax + 1;
    M = yMax + 1;
    N_preview = (int)(N / R_preview);
    M_preview = (int)(M / R_preview);

    matrix.init(N, M);
    matrixp.init(N_preview, M_preview);

    //int MW = matrix.getLength(0);// a kiinduló, nagy mátrix adatai
    //int MH = matrix.getLength(1);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            matrix.setData(i, j, -1);
    }

    setFilter(&matrix, lines);

    for (int i = 0; i < N_preview; i++)
    {
        for (int j = 0; j < M_preview; j++)
        {
            double aa;
            bool okaa = matrix.data((int)(i * R_preview),
                                  (int)(j * R_preview), &aa);
            if(!okaa) continue;
            matrixp.setData(i, j, aa);
        }
    }

    setMul(lines, csvMultiplier);

    if (mul.count() != sensors.count())
    {
        for (auto& item_Key: sensors.keys())
        {
            if (!mul.contains(item_Key)) mul.insert(item_Key, 1);
        }
    }

    isInited = true;
}

QColor BitMapGen::GrayLevel(double v)
{
    double r = v * 16;
    if (r < 0) r = 0;
    if (r > 255) r = 255;

    unsigned char g = (unsigned char)r;
    return QColor::fromRgb(g, g, g,(unsigned char)255);
}

        //public double[,] getPressureMap(Dictionary<int, int> values2)
        //{
        //    if (!isInited) return null;
        //    double[,] m2;
        //    if (preview)
        //    {
        //        m2 = matrixp.Clone() as double[,];
        //    }
        //    else
        //    {
        //        m2 = matrix.Clone() as double[,];
        //    }

        //    for (int ch = 1; ch < 16; ch++)
        //    {
        //        int pressureV = values2[ch] / 64;
        //        ipolMat(ref m2, sensors, pressureV, ch);
        //    }

        //    return m2;
        //}

        //public double[,] getPressureMap2_old(int[] values2)
        //{
        //    if (!isInited) return null;
        //    double[,] m2;
        //    if (preview) m2 = matrixp.Clone() as double[,];
        //    else m2 = matrix.Clone() as double[,];

        //    for (int i = 0; i < values2.Length; i++) ipolMat(ref m2, sensors, values2[i] / 64, i + 1);
        //    return m2;
        //}

DoubleMatrix BitMapGen::getPressureMap2(const QVarLengthArray<int>& values2)
{
    DoubleMatrix a = _preview
            ?getPressureMap3_preview(values2)
           :getPressureMap3(values2);
    //a.mirror();
    return a;
}

DoubleMatrix BitMapGen::getPressureMap3_preview(const QVarLengthArray<int>& values2)
{
    if (!isInited) return {};
    if (values2.length()==0) return {};

    DoubleMatrix m2(matrixp);
    m2.init(N_preview, M_preview);
    for(int i=0,L=values2.length();i<L;i++){
        auto v = values2[i] / 64;
        int ch = i + 1;
        ipolMat_preview(&m2, v, ch);
    }
    return m2;
}

DoubleMatrix BitMapGen::getPressureMap3(const QVarLengthArray<int>& pressures)
{
    if (!isInited) return {};
    if (pressures.length() == 0) return {};

    DoubleMatrix m2(matrix);

    for(int i=0,L=pressures.length();i<L;i++){
        auto v = pressures[i] / 64;
        int ch = i + 1;
        ipolMat(&m2, v, ch);
    }
    return m2;
}

QImage BitMapGen::getColoredBitmap(const DoubleMatrix& m2, double limit, bool isColored, bool isTransparent, QColor backgroundColor)
{
    //if (!isInited) return {};
    //if (m2 == null) return null;
    int MW = m2.getLength(0);// a kiinduló, nagy mátrix adatai
    int MH = m2.getLength(1);
    if (MW == 0) return {};
    if (MH == 0) return {};

//    const BitmapPixelFormat PIXFM = BitmapPixelFormat.Rgba8;

    //SoftwareBitmap newBitmap = new SoftwareBitmap(PIXFM, MW, MH, BitmapAlphaMode.Premultiplied);
    QImage b(MW, MH, QImage::Format_RGBA8888_Premultiplied);
    //using (BitmapBuffer bm1Data = newBitmap.LockBuffer(BitmapBufferAccessMode.Write))
    //{
    //    using (IMemoryBufferReference reference = bm1Data.CreateReference())
    //    {
       //     ((IMemoryBufferByteAccess)reference).GetBuffer(out byte* dataInBytes, out _);

       //     BitmapPlaneDescription bufferLayout = bm1Data.GetPlaneDescription(0);

          //  Parallel.For(0, bufferLayout.Height, (i, state) =>
            for(int i=0;i<MH; i++)
            {
                uchar* u = b.scanLine(i);
                //int actialStride = bufferLayout.StartIndex + (bufferLayout.Stride * i);
                //int rowIx;
                QColor c;

                uint s = 0;//actialStride;
                for (int j = 0; j < MW; j++)
                {
                    //rowIx = bufferLayout.Height - 1 - i;
                    double v=0;
                    bool okv = m2.data(j, i, &v);
                    if (!okv || v < 0)
                    {
                        u[s++] = backgroundColor.red();
                        u[s++] = backgroundColor.green();
                        u[s++] = backgroundColor.blue();
                        u[s++] = isTransparent ? 0 : 255;
                    }
                    else
                    {
                        c = isColored
                                ? GetColorByLevel(v, limit, backgroundColor)
                               : GrayLevel(v);

                        u[s++] = c.red();
                        u[s++] = c.green();
                        u[s++] = c.blue();
                        u[s++] = 255;
                    }
                }
            }//);
   //     }
  //  }

    return b;
}

//igény van arra, hogy a mátrix cellája négyzetes legyen, ezért megadjuk, hogy vízszintesen n darab van, majd kiszámoljuk, hogy föggőlegesen mennyinek kell lennie
// n: ennyi cella van vízszintesen
// m: ennyi cella lesz függőlegesen
DoubleMatrix BitMapGen::ScaleMatrix(const DoubleMatrix& pressuremap, int n)
{
    //if (pressuremap == null) return null;
    int MW = pressuremap.getLength(0);// a kiinduló, nagy mátrix adatai
    int MH = pressuremap.getLength(1);

    double w = ((double)MW) / n; // ennyi pixel egy cella
    int m = (int)qCeil(MH / w); //ennyi cella van függőlegesen

    //double w2 = r.Width / n;
    //int m2 = (int)Math.Ceiling(r.Height / w2);
    DoubleMatrix e;
    e.init(n, m); //e mint eredmény

    const int mm = 4;

    double ww = w / mm;

    for (int i = 0; i < m; i++)//függőleges
    //Parallel.For(0, m, (i, status) =>
    {
        //var y = tl.Y + i * w2;
        int y0 = MH - 1 - (int)(i * w);

        for (int j = 0; j < n; j++)//vízszintes
        {
            //var x = tl.X + j * w2;
            int x0 = (int)(j * w);

            double vv = 0;
            double vn = 0;
            for (int a = 1; a < mm; a++)
            {
                int ya = y0 - (int)(a * ww);
                if (ya < 0) continue;

                for (int b = 1; b < mm; b++)
                {
                    int xb = x0 + (int)(b * ww);
                    if (xb < 0) continue;

                    double v1;
                    bool okv1  = pressuremap.data(xb, ya, &v1);
                    if (okv1 && v1 >= 0) // a szélét és a keretet nem adjuk hozzá
                    {
                        vv += v1;
                        vn++;
                    }
                }
            }

            double v = vv / vn;

            e.setData(j, i, v);
        }
    }
    return e;
}

// 16->100%
QColor BitMapGen::GetColorByLevel(double v, double limit, QColor backgroundColor)
{
    if(_rgb.empty())
        return Qt::red;
    if (v < 0) return backgroundColor;
    //if (limit > 0 && v < limit) return _rgb[0];
    if (v > 15.75)
        return _rgb[12];
    if (v > 15) return _rgb[11];
    if (v > 14.25) return _rgb[10];
    if (v > 13.5) return _rgb[9];
    if (v > 11) return _rgb[8];
    if (v > 8.5) return _rgb[7];//zöld
    if (v > 5.5) return _rgb[6];
    if (v > 5) return _rgb[5];
    if (v > 4.5) return _rgb[4];
    if (v > 4) return _rgb[3];
    if (v > 3) return _rgb[2];
    if (v > 2)
        return _rgb[1];

    return _rgb[0];
}

void BitMapGen::ipolMat_preview(DoubleMatrix* matrix,
                                int pressureV,
                                int ch)
{
    int n = sensors[ch].n;
    int m = sensors[ch].m;
    auto v2 = sensors[ch].lut[pressureV];

    for (int i = 0; i < n; i += (int)R_preview)
    {
        int x2 = (int)((sensors[ch].p.x - (n / 2) + i) / R_preview);
        for (int j = 0; j < m; j += (int)R_preview)
        {
            int y2 = (int)((sensors[ch].p.y - (m / 2) + j) / R_preview);
            double v1;
            bool okv1 = matrix->data(x2, y2, &v1);
            if(!okv1) continue;
            bool ok = (x2 >= 0)
            && (x2 < N_preview)
            && (y2 >= 0)
            && (y2 < M_preview)
            && (v1 >= 0);
            if (!ok) continue;
            double v3;
            bool okv3 = v2.data(i, j, &v3) * mul[ch];
            if(!okv3) continue;
            matrix->setData(x2, y2, v1 + v3);
        }
    }
}

void BitMapGen::ipolMat(DoubleMatrix* matrix, int pressureV, int ch)
{
    int n = sensors[ch].n;
    int m = sensors[ch].m;
    DoubleMatrix v2 = sensors[ch].lut[pressureV];

    for (int i = 0; i < n; i++)
    {
        int x2 = (int)(sensors[ch].p.x - (n / 2) + i);
        for (int j = 0; j < m; j++)
        {
            double v1=0;
            int y2 = (int)(sensors[ch].p.y - (m / 2) + j);
            bool okv1 = matrix->data(x2, y2, &v1);
            //if(!okv1) continue;
            bool ok = (x2 >= 0)
            && (x2 < N)
            && (y2 >= 0)
            && (y2 < M)
            && (v1 >= 0);
            if (!ok) continue;
            double v3=0;
            bool okv3 = v2.data(i, j, &v3)* mul[ch];
            if(!okv3) continue;
            matrix->setData(x2, y2, v1 + v3);
        }
    }
}


        //körvonal -> EdgeTable
        //42j16sf;none;25;275;none;none
        //42J16s
        //42B16s
void BitMapGen::setFilter(DoubleMatrix* matrix, const QStringList& pts)
{
    EdgeTableTuple ActiveEdgeTuple;// = new EdgeTableTuple();
    EdgeTableTuple EdgeTable[M];
    ActiveEdgeTuple.buckets.resize(maxVer);
    for (int i = 0; i < M; i++)
    {
        EdgeTable[i].buckets.resize(maxVer);
    }
    initEdgeTable(EdgeTable, &ActiveEdgeTuple);
    //List<string> pts;
    //pts = File.ReadAllLines(filename).ToList();
    int count = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    for (int i = 1; i < pts.length(); i++)
    {
        QStringList token = pts[i].split(';');
        if (token.length() != 6) continue;
        if (token[1] != "none") continue;
        //if (side == "B" && token[0] != "42fb16s" || side == "J" && token[0] != "42fj16s") continue;
        QString t0 = token[0].toLower().trimmed();
        //if (t0 != insolename + 'f') continue;
        if (!t0.endsWith('f')) continue;
        count++;
        if (count > 2)
        {
            x1 = x2;
            y1 = y2;
            count = 2;
        }
        if (count == 1)
        {
            x1 = token[2].toInt();
            y1 = token[3].toInt();
        }
        else
        {
            x2 = token[2].toInt();
            y2 = token[3].toInt();
            storeEdgeInTable(EdgeTable, y1 + 1, x1 + 1, y2 + 1, x2 + 1);//storage of edges in edge table.
        }
    }

    ScanlineFill(matrix, EdgeTable, &ActiveEdgeTuple);
}

void BitMapGen::ScanlineFill(DoubleMatrix* matrix, EdgeTableTuple* EdgeTable, EdgeTableTuple* ActiveEdgeTuple)
{
    int i, j, x1, ymax1, x2, ymax2, coordCount;
    bool FillFlag;// = false;

    // we will start from scanline 0;
    // Repeat until last scanline:
    for (i = 0; i < M; i++)//4. Increment y by 1 (next scan line)
    {

        // 1. Move from ET bucket y to the
        // AET those edges whose ymin = y (entering edges)
        for (j = 0; j < EdgeTable[i].countEdgeBucket; j++)
        {
            storeEdgeInTuple(ActiveEdgeTuple, EdgeTable[i].buckets[j].
                     ymax, (int)EdgeTable[i].buckets[j].xOfMin,
                    EdgeTable[i].buckets[j].slopeInverse);
        }

        // 2. Remove from AET those edges for
        // which y=ymax (not involved in next scan line)
        removeEdgeByYmax(ActiveEdgeTuple, i);

        //sort AET (remember: ET is presorted)
        insertionSort(ActiveEdgeTuple);

        //3. Fill lines on scan line y by using pairs of x-coords from AET
        j = 0;
        //FillFlag = false;
        coordCount = 0;
        x1 = 0;
        x2 = 0;
        ymax1 = 0;
        ymax2 = 0;
        while (j < ActiveEdgeTuple->countEdgeBucket)
        {
            if (coordCount % 2 == 0)
            {
                x1 = (int)(ActiveEdgeTuple->buckets[j].xOfMin);
                ymax1 = ActiveEdgeTuple->buckets[j].ymax;
                if (x1 == x2)
                {
                    // three cases can arrive-
                    //  1. lines are towards top of the intersection
                    //  2. lines are towards bottom
                    //  3. one line is towards top and other is towards bottom
                    if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
                    {
                        x2 = x1;
                        ymax2 = ymax1;
                    }

                    else
                    {
                        coordCount++;
                    }
                }

                else
                {
                    coordCount++;
                }
            }
            else
            {
                x2 = (int)ActiveEdgeTuple->buckets[j].xOfMin;
                ymax2 = ActiveEdgeTuple->buckets[j].ymax;

                FillFlag = false;

                // checking for intersection...
                if (x1 == x2)
                {
                    //  three cases can arive-
                    //  1. lines are towards top of the intersection
                    //  2. lines are towards bottom
                    //  3. one line is towards top and other is towards bottom
                    if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
                    {
                        x1 = x2;
                        ymax1 = ymax2;
                    }
                    else
                    {
                        coordCount++;
                        FillFlag = true;
                    }
                }
                else
                {
                    coordCount++;
                    FillFlag = true;
                }


                if (FillFlag)
                {
                    //drawing actual lines...
                    for (int k = x1; k < x2; k++)
                    {
                        matrix->setData(i, k, 0);
                    }
                    //printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i);
                }

            }

            j++;
        }


        // 5. For each nonvertical edge remaining in AET, update x for new y
        updatexbyslopeinv(ActiveEdgeTuple);
    }
}

void BitMapGen::updatexbyslopeinv(EdgeTableTuple* Tup)
{
    int i;

    for (i = 0; i < Tup->countEdgeBucket; i++)
    {
        (Tup->buckets[i]).xOfMin += (Tup->buckets[i]).slopeInverse;
    }
}

void BitMapGen::removeEdgeByYmax(EdgeTableTuple* Tup, int yy)
{
    int i, j;
    for (i = 0; i < Tup->countEdgeBucket; i++)
    {
        if (Tup->buckets[i].ymax == yy)
        {
            for (j = i; j < Tup->countEdgeBucket - 1; j++)
            {
                Tup->buckets[j].ymax = Tup->buckets[j + 1].ymax;
                Tup->buckets[j].xOfMin = Tup->buckets[j + 1].xOfMin;
                Tup->buckets[j].slopeInverse = Tup->buckets[j + 1].slopeInverse;
            }
            Tup->countEdgeBucket--;
            i--;
        }
    }
}

void BitMapGen::storeEdgeInTable(EdgeTableTuple EdgeTable[], int x1, int y1, int x2, int y2)
{
    double m, minv;
    int ymaxTS, xwithyminTS, scanline; //ts stands for to store

    if (x2 == x1)
    {
        minv = 0.000000;
    }
    else
    {
        m = ((float)(y2 - y1)) / ((float)(x2 - x1));

        // horizontal lines are not stored in edge table
        if (y2 == y1)
            return;

        minv = (float)1.0 / m;
    }

    if (y1 > y2)
    {
        scanline = y2;
        ymaxTS = y1;
        xwithyminTS = x2;
    }
    else
    {
        scanline = y1;
        ymaxTS = y2;
        xwithyminTS = x1;
    }
    // the assignment part is done..now storage..

    storeEdgeInTuple(&EdgeTable[scanline], ymaxTS, xwithyminTS, minv);
}

void BitMapGen::storeEdgeInTuple(EdgeTableTuple* receiver, int ym, int xm, double slopInv)
{
    receiver->buckets[receiver->countEdgeBucket].ymax = ym;
    receiver->buckets[receiver->countEdgeBucket].xOfMin = (float)xm;
    receiver->buckets[receiver->countEdgeBucket].slopeInverse = slopInv;

    insertionSort(receiver);

    receiver->countEdgeBucket++;
}

void BitMapGen::insertionSort(EdgeTableTuple* ett)
{
    int i, j;
    EdgeBucket temp;

    for (i = 1; i < ett->countEdgeBucket; i++)
    {
        temp.ymax = ett->buckets[i].ymax;
        temp.xOfMin = ett->buckets[i].xOfMin;
        temp.slopeInverse = ett->buckets[i].slopeInverse;
        j = i - 1;

        while ((j >= 0) && (temp.xOfMin < ett->buckets[j].xOfMin))
        {
            ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
            ett->buckets[j + 1].xOfMin = ett->buckets[j].xOfMin;
            ett->buckets[j + 1].slopeInverse = ett->buckets[j].slopeInverse;
            j--;
        }
        ett->buckets[j + 1].ymax = temp.ymax;
        ett->buckets[j + 1].xOfMin = temp.xOfMin;
        ett->buckets[j + 1].slopeInverse = temp.slopeInverse;
    }
}

void BitMapGen::initEdgeTable(EdgeTableTuple EdgeTable[],
                              EdgeTableTuple* ActiveEdgeTuple)
{
    for (int i = 0; i < M; i++)
    {
        EdgeTable[i].countEdgeBucket = 0;
    }

    ActiveEdgeTuple->countEdgeBucket = 0;
}

void BitMapGen::listToMap()
{
    if (sensors.count() == 0) return;
    auto keys = sensors.keys();

    for(auto& key:keys)
    {
        sensors[key].name = QString::number(sensors[key].p.ch);
        generatePt2(key);
    }
}

void BitMapGen::generatePt2(int key)
{
    sensors[key].n = sensors[key].m = (int)(rMult * 2 * sensors[key].p.r);
    //sensors[key].lut;// = QList<DoubleMatrix>();
    generateMat(key);

    /*sensors[key].r = param.r;
    sensors[key].ratio = param.ratio;
    sensors[key].x = param.x;
    sensors[key].y = param.y;*/

    //---------------------------------------

    QList<DoubleMatrix> lut2;// = new List<double[,]>();

    if (sensors[key].p.ratio < 1)
    {
        sensors[key].n = (int)(sensors[key].p.ratio * sensors[key].n);
        double b = 1 / sensors[key].p.ratio;
        for (int intensity = 0; intensity < intLvls; intensity++)
        {
            DoubleMatrix mat;
            mat.init(sensors[key].n, sensors[key].m);
            auto a = sensors[key].lut[intensity];

            for (int i = 0; i < sensors[key].n; i++)
            {
                for (int j = 0; j < sensors[key].m; j++)
                {
                    double aa;
                    bool okaa = a.data((int)(i * b), j, &aa);
                    if(!okaa) continue;
                    mat.setData(i, j, aa);
                }
            }
            lut2.append(mat);
        }
    }
    if (sensors[key].p.ratio > 1)
    {
        sensors[key].m = (int)(sensors[key].p.ratio * sensors[key].m);
        for (int intensity = 0; intensity < intLvls; intensity++)
        {
            DoubleMatrix mat;
            mat.init(sensors[key].n, sensors[key].m);
            auto a = sensors[key].lut[intensity];
            for (int i = 0; i < sensors[key].n; i++)
            {
                for (int j = 0; j < sensors[key].m; j++)
                {
                    double aa;
                    bool okaa = a.data((int)(i)
                                       ,(int)(j * sensors[key].p.ratio)
                                       , &aa);
                    if(!okaa) continue;
                    mat.setData(i, j, aa);
                }
            }
            lut2.append(mat);
        }
    }

    sensors[key].lut = lut2;

    //---------------------------------------
}

void BitMapGen::generateMat(int key)
{
    //if (!isInited) return;
    for (int intensity = 0; intensity < intLvls; intensity++)
    {
        DoubleMatrix mat;
        mat.init(sensors[key].n, sensors[key].m);
        for (int i = 0; i < sensors[key].n; i++)
        {
            for (int j = 0; j < sensors[key].m; j++)
            {
                mat.setData(i, j, 0);
            }
        }

        BivariateNormalPdf(&mat, intensity + 1, sensors[key].n);

        sensors[key].lut.append(mat);
    }
}

void BitMapGen::BivariateNormalPdf(DoubleMatrix* matrix, int height, int n)
{
    double mu[]{ 0, 0 };

    DoubleMatrix sigma;
    sigma.init(2,2);
    sigma.setData(0,0,n * szoras);
    sigma.setData(0,1,0);
    sigma.setData(1,0,0);
    sigma.setData(1,1,n * szoras);

    DoubleMatrix isigma;
    isigma.init(2,2);

    invMat(sigma, &isigma, 2);
    double detSigma = determinantOfMatrix(sigma, 2);
    double coeff = 1 / (2 * M_PI * qSqrt(detSigma));

    double max = 0;

    //bool high = ratio < 1 ? true : false;
    //int skipCond = (int)(high ? 1 / ratio : ratio);

    for (int i = 0; i < n; i++)
    {
        double x = i - (n / 2.0);
        for (int j = 0; j < n; j++)
        {
            double y = j - (n / 2.0);
            double p[]{ x, y };
            double pm[2];
            pm[0] = p[0] - mu[0];
            pm[1] = p[1] - mu[1];
            double s00, s01, s10, s11;
            isigma.data(0, 0, &s00);
            isigma.data(0, 1, &s01);
            isigma.data(1, 0, &s10);
            isigma.data(1, 1, &s11);
            double a = (pm[0] * s00) + (pm[1] * s01);
            double b = (pm[0] * s10) + (pm[1] * s11);
            double c = p[0] * a;
            double d = p[1] * b;
            double v = coeff * qExp(-0.5 * (c + d));
            matrix->setData(i, j, v);

            //matrix->data(i, j)
            if (v > max)
                max = v;//matrix->data(i, j);
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double v1;
            matrix->data(i, j, &v1);
            double v2 = height / max;
            matrix->setData(i, j, v1 * v2);
        }
    }
}

double BitMapGen::determinantOfMatrix(const DoubleMatrix& mat, int n)
{
    int D = 0; // Initialize result

    //  Base case : if matrix contains single element
    if (n == 1){
        double v;
        mat.data(0, 0, &v);
        return v;
    }

    DoubleMatrix temp;
    temp.init(n, n);

    int sign = 1;  // To store sign multiplier

    // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of mat[0][f]
        getCofactor(mat, &temp, 0, f, n);
        double v1;
        mat.data(0, f, &v1);

        D += (int)(sign * v1 * determinantOfMatrix(temp, n - 1));

        // terms are to be added with alternate sign
        sign = -sign;
    }

    return D;
}

void BitMapGen::getCofactor(const DoubleMatrix& mat, DoubleMatrix* temp, int p, int q, int n)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                double v;
                mat.data(row, col, &v);
                temp->setData(i, j++, v);

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

bool BitMapGen::invMat(const DoubleMatrix& mat, DoubleMatrix* inv, int a)
{
    double det = determinantOfMatrix(mat, a);
    if (det == 0)
    {
        return false;
    }
    DoubleMatrix adj;
                        adj.init(a, a);
    adjMat(mat, &adj, a);
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < a; j++)
        {
            double v0;
            adj.data(i, j, &v0);
            double v = v0 / det;
            inv->setData(i, j, v);
        }
    }
    return true;
}

void BitMapGen::adjMat(const DoubleMatrix& mat, DoubleMatrix* adj, int a)
{
    if (a == 1)
    {
        adj->setData(0, 0, 1); return;
    }
    int s;// = 1;
    DoubleMatrix t;
    t.init(a, a);
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < a; j++)
        {
            //To get cofactor of M[i][j]
            getCofactor(mat, &t, i, j, a);
            s = ((i + j) % 2 == 0) ? 1 : -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
            double v = s * (determinantOfMatrix(t, a - 1)); //Interchange rows and columns to get the transpose of the cofactor matrix
            adj->setData(j, i, v);
        }
    }
}

void BitMapGen::setMaxMin(const QStringList& s)
{
    //List<string> s = File.ReadAllLines(filename).ToList();
    for (int i = 1; i < s.length(); i++)
    {
        if (s[i].startsWith('#')) continue;
        if (s[i].isEmpty()) continue;

        QStringList token = s[i].split(';');
        if (!token[0].endsWith("f")) continue;
        int x = token[2].toInt();
        int y = token[3].toInt();

        if (x > xMax) xMax = x;
        if (y > yMax) yMax = y;
        if (x < xMin) xMin = x;
        if (y < yMin) yMin = y;
    }

    xMax += 2;
    yMax += 2;
}

void BitMapGen::fromCSV(const QStringList& s)
{
    //List<string> s = File.ReadAllLines(filename).ToList();
    //List<pt> l = new List<pt>();

    for (int i = 1; i < s.length(); i++)
    {
        if (s[i].startsWith('#')) continue;
        if (s[i].isEmpty()) continue;

        QStringList token = s[i].split(';');
        if (token.length() != 6) continue;
        //if (side == "B" && token[0] != "42b16s" || side == "J" && token[0] != "42j16s")
        //    continue;
        if (token[1] == "none") continue;
        QString t0 = token[0].toLower().trimmed();//zNormalize();
        //if (t0 != insolename) continue;
        if (!t0.endsWith('s')) continue;
        pt p;
        p.x = token[2].toInt();
        p.y = token[3].toInt();
        //p.r = Convert.ChangeType(token[2].Trim().Replace(',', '.'), t, new CultureInfo("en-US"));// double.Parse(token[2]);
        //p.r = Convert.ToDouble(token[4].Trim().Replace(',', '.'), new CultureInfo("en-US"));
        p.r = token[4].toDouble();
        p.ch = token[1].toInt();
        p.ratio = token[5].toDouble();
        //p.ratio = Convert.ToDouble(token[5].Trim().Replace(',', '.'), new CultureInfo("en-US"));
        //l.Add(p);
        pt2 p2;
        p2.p = p;
        sensors.insert(p.ch, p2);
    }

    //return l;
}

void BitMapGen::setMul(const QStringList& s, double csvMultiplier)
{
    //mul = QMap<int, double>();

    for (int i = 1; i < s.length(); i++)
    {
        QStringList token = s[i].split(';');
        if (token.length() != 6) continue;
        if (token[1] == "none") continue;
        QString t0 = token[0].toLower().trimmed();//zNormalize();
        //if (t0 != insolename + "w") continue;
        if (!t0.endsWith('w')) continue;
        int sensor_id = token[1].toInt();
        double multiplier = token[2].toDouble();
        if (csvMultiplier != 0 && csvMultiplier != 1) multiplier *= csvMultiplier;
        mul.insert(sensor_id, multiplier);
    }
}




        /*
         *  if (v > 16) return rgb[12];
            if (v > 14.6) return rgb[11];
            if (v > 14.1) return rgb[10];
            if (v > 13.4) return rgb[9];
            if (v > 12.5) return rgb[8];
            if (v > 11.4) return rgb[7];
            if (v > 10.1) return rgb[6];
            if (v > 8.6) return rgb[5];
            if (v > 6.9) return rgb[4];
            if (v > 5) return rgb[3];
            if (v > 4) return rgb[2];
            if (v > .6) return rgb[1];
            if (v < 0) return Color.FromArgb(0, 0, 0, 0);
            else return rgb[0];
         */
        //public PointF GetCenterOfGravity(Dictionary<int, int> values)//4
        //{
        //    if (!isInited) return PointF.Empty;
        //    float x=0, y=0, w=0;
        //    foreach (var s in sensors)
        //    {
        //        var v = values[s.Key];
        //        if (v < 10*64) continue;//442
        //        x += s.Value.p.x * v;
        //        y += s.Value.p.y * v;
        //        w += v;
        //    }
        //    if (w == 0) return PointF.Empty;

        //    return new PointF(x/w, y/w);
        //}
    //}

