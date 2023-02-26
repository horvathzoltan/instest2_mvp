#include "bitmapgen.h"
#include "logger.h"


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


void BitMapGen::Init(QString insoleMetaContent, int _prevRes,
          double csvMultiplier, QColor backgroundColor)
{
    if (_isInitializing) return;
    _isInitializing = true;

    Init2(insoleMetaContent, _prevRes, csvMultiplier, backgroundColor);
    _isInitializing = false;
}

void BitMapGen::Init2(QString insoleMetaContent,
                      int _prevRes, double csvMultiplier,
                      QColor bkcolor)
{
    isInited = false;
    if (insoleMetaContent.isEmpty()) return;
    R_preview = _prevRes;
    //insolename = insolename.zNormalize();
    _backgroundColor = bkcolor;

    auto lines = insoleMetaContent.split('\n');

    for(auto& item:c)
    {
        QColor x(item);
        rgb.append(x);
    }

    xMax = yMax = std::numeric_limits<int>::min();
    xMin = yMin = std::numeric_limits<int>::max();

    setMaxMin(lines);
    //FootprintSideEnum.Left

    /////
    sensors = QMap<int, pt2>();
    fromCSV(lines);
    if (sensors.count() == 0)
    {
        //throw new Exception(TranslationService.Translate(TranslateEnum.NOINSOLEDATAINFILE, filename));
        return;
    }
    /// ha itt jön egy dispose
    listToMap();
    ///////////
    N = xMax + 1;
    M = yMax + 1;
    N_preview = (int)(N / R_preview);
    M_preview = (int)(M / R_preview);

    matrix = new double[N, M];
    matrixp = new double[N_preview, M_preview];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            matrix[i, j] = -1;
    }

    setFilter(ref matrix, lines);

    for (int i = 0; i < N_preview; i++)
    {
        for (int j = 0; j < M_preview; j++)
            matrixp[i, j] = matrix[(int)(i * R_preview), (int)(j * R_preview)];
    }

    setMul(lines, csvMultiplier);

    if (mul.Count != sensors.Count)
    {
        foreach (KeyValuePair<int, pt2> item in sensors)
        {
            if (!mul.ContainsKey(item.Key)) mul.Add(item.Key, 1);
        }
    }

    isInited = true;
}

        public static Color GrayLevel(double v)
        {
            double r = v * 16;
            if (r < 0) r = 0;
            if (r > 255) r = 255;

            byte g = (byte)r;
            return Color.FromArgb((byte)255, g, g, g);
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

        public double[,] getPressureMap2(int[] values2)
        {
            if (preview) return getPressureMap3_preview(values2);
            return getPressureMap3(values2);
        }

        public double[,] getPressureMap3_preview(int[] values2)
        {
            if (!isInited) return null;
            if (values2?.Length == 0) return null;

            double[,] m2 = matrixp.Clone() as double[,];
            Parallel.For(0, values2.Length, (i) => ipolMat_preview(ref m2, sensors, values2[i] / 64, i + 1));
            return m2;
        }

        public double[,] getPressureMap3(int[] values2)
        {
            if (!isInited) return null;
            if (values2?.Length == 0) return null;

            double[,] m2 = matrix.Clone() as double[,];
            Parallel.For(0, values2.Length, (i) => ipolMat(ref m2, sensors, values2[i] / 64, i + 1));
            return m2;
        }

        public unsafe SoftwareBitmap getColoredBitmap(double[,] m2, double limit, bool isColored, bool isTransparent)
        {
            if (!isInited) return null;
            if (m2 == null) return null;
            int MW = m2.GetLength(0);// a kiinduló, nagy mátrix adatai
            int MH = m2.GetLength(1);
            if (MW == 0) return null;
            if (MH == 0) return null;

            const BitmapPixelFormat PIXFM = BitmapPixelFormat.Rgba8;

            SoftwareBitmap newBitmap = new SoftwareBitmap(PIXFM, MW, MH, BitmapAlphaMode.Premultiplied);

            using (BitmapBuffer bm1Data = newBitmap.LockBuffer(BitmapBufferAccessMode.Write))
            {
                using (IMemoryBufferReference reference = bm1Data.CreateReference())
                {
                    ((IMemoryBufferByteAccess)reference).GetBuffer(out byte* dataInBytes, out _);

                    BitmapPlaneDescription bufferLayout = bm1Data.GetPlaneDescription(0);

                    Parallel.For(0, bufferLayout.Height, (i, state) =>
                    {
                        int actialStride = bufferLayout.StartIndex + (bufferLayout.Stride * i);
                        int rowIx;
                        Color c;
                        double v;
                        int s = actialStride;
                        for (int j = 0; j < bufferLayout.Width; j++)
                        {
                            rowIx = bufferLayout.Height - 1 - i;
                            v = m2[j, rowIx];
                            if (v < 0)
                            {
                                dataInBytes[s++] = _backgroundColor.R;
                                dataInBytes[s++] = _backgroundColor.G;
                                dataInBytes[s++] = _backgroundColor.B;
                                dataInBytes[s++] = isTransparent ? (byte)0 : (byte)255;
                            }
                            else
                            {
                                c = isColored ? GetColorByLevel(v, limit) : GrayLevel(v);

                                dataInBytes[s++] = c.R;
                                dataInBytes[s++] = c.G;
                                dataInBytes[s++] = c.B;
                                dataInBytes[s++] = (byte)255;
                            }
                        }
                    });
                }
            }

            return newBitmap;
        }

        //igény van arra, hogy a mátrix cellája négyzetes legyen, ezért megadjuk, hogy vízszintesen n darab van, majd kiszámoljuk, hogy föggőlegesen mennyinek kell lennie
        // n: ennyi cella van vízszintesen
        // m: ennyi cella lesz függőlegesen
        public static double[,] ScaleMatrix(double[,] pressuremap, int n)
        {
            if (pressuremap == null) return null;
            int MW = pressuremap.GetLength(0);// a kiinduló, nagy mátrix adatai
            int MH = pressuremap.GetLength(1);

            double w = ((double)MW) / n; // ennyi pixel egy cella
            int m = (int)Math.Ceiling(MH / w); //ennyi cella van függőlegesen

            //double w2 = r.Width / n;
            //int m2 = (int)Math.Ceiling(r.Height / w2);
            double[,] e = new double[n, m]; //e mint eredmény

            const int mm = 4;

            double ww = w / mm;

            //for (int i = 0; i < m; i++)//függőleges
            Parallel.For(0, m, (i, status) =>
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

                            double v1 = pressuremap[xb, ya];
                            if (v1 >= 0) // a szélét és a keretet nem adjuk hozzá
                            {
                                vv += v1;
                                vn++;
                            }
                        }
                    }

                    double v = vv / vn;

                    e[j, i] = v;
                }
            });
            return e;
        }

        /*
         if (v > 16) return rgb[12];
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
            //if (v < 0) return Color.FromArgb(0, 0, 0, 0);
            else return rgb[0];
            //return rgb[GetLevelIx(v)];
            //return RGBHelper.Colors[GetLevelIx(v)];*/



        // 16->100%
        private Color GetColorByLevel(double v, double limit = -1)
        {
            if (v < 0) return _backgroundColor;
            if (limit > 0 && v < limit) return rgb[0];
            if (v > 15.75)
                return rgb[12];
            if (v > 15) return rgb[11];
            if (v > 14.25) return rgb[10];
            if (v > 13.5) return rgb[9];
            if (v > 11) return rgb[8];
            if (v > 8.5) return rgb[7];//zöld
            if (v > 5.5) return rgb[6];
            if (v > 5) return rgb[5];
            if (v > 4.5) return rgb[4];
            if (v > 4) return rgb[3];
            if (v > 3) return rgb[2];
            if (v > 2)
                return rgb[1];

            return rgb[0];
        }
        private void ipolMat_preview(ref double[,] matrix, Dictionary<int, pt2> sensors, int pressureV, int ch)
        {
            int n = sensors[ch].n;
            int m = sensors[ch].m;
            double[,] v2 = sensors[ch].lut[pressureV];

            for (int i = 0; i < n; i += (int)R_preview)
            {
                int x2 = (int)((sensors[ch].p.x - (n / 2) + i) / R_preview);
                for (int j = 0; j < m; j += (int)R_preview)
                {
                    int y2 = (int)((sensors[ch].p.y - (m / 2) + j) / R_preview);
                    bool ok = (x2 >= 0) && (x2 < N_preview) && (y2 >= 0) && (y2 < M_preview) && (matrix[x2, y2] >= 0);
                    if (!ok) continue;
                    double v1 = matrix[x2, y2];
                    double v3 = v2[i, j] * mul[ch];
                    matrix[x2, y2] = v1 + v3;
                }
            }
        }

        private void ipolMat(ref double[,] matrix, Dictionary<int, pt2> sensors, int pressureV, int ch)
        {
            int n = sensors[ch].n;
            int m = sensors[ch].m;
            double[,] v2 = sensors[ch].lut[pressureV];

            for (int i = 0; i < n; i++)
            {
                int x2 = (int)(sensors[ch].p.x - (n / 2) + i);
                for (int j = 0; j < m; j++)
                {
                    int y2 = (int)(sensors[ch].p.y - (m / 2) + j);
                    bool ok = (x2 >= 0) && (x2 < N) && (y2 >= 0) && (y2 < M) && (matrix[x2, y2] >= 0);
                    if (!ok) continue;
                    double v1 = matrix[x2, y2];
                    double v3 = v2[i, j] * mul[ch];
                    matrix[x2, y2] = v1 + v3;
                }
            }
        }


        //körvonal -> EdgeTable
        //42j16sf;none;25;275;none;none
        //42J16s
        //42B16s
        private void setFilter(ref double[,] matrix, string[] pts)
        {
            EdgeTableTuple ActiveEdgeTuple = new EdgeTableTuple();
            EdgeTableTuple[] EdgeTable = new EdgeTableTuple[M];
            ActiveEdgeTuple.buckets = new EdgeBucket[maxVer];
            for (int i = 0; i < M; i++)
            {
                EdgeTable[i].buckets = new EdgeBucket[maxVer];
            }
            initEdgeTable(ref EdgeTable, ref ActiveEdgeTuple);
            //List<string> pts;
            //pts = File.ReadAllLines(filename).ToList();
            int count = 0, x1 = 0, y1 = 0, x2 = 0, y2 = 0;

            for (int i = 1; i < pts.Length; i++)
            {
                string[] token = pts[i].Split(';');
                if (token.Length != 6) continue;
                if (token[1] != nameof(Messages.token.none)) continue;
                //if (side == "B" && token[0] != "42fb16s" || side == "J" && token[0] != "42fj16s") continue;
                string t0 = token[0].zNormalize();
                //if (t0 != insolename + 'f') continue;
                if (!t0.EndsWith('f')) continue;
                count++;
                if (count > 2)
                {
                    x1 = x2;
                    y1 = y2;
                    count = 2;
                }
                if (count == 1)
                {
                    x1 = CSVImportHelper.ToNumber<int>(token[2]);
                    y1 = CSVImportHelper.ToNumber<int>(token[3]);
                }
                else
                {
                    x2 = CSVImportHelper.ToNumber<int>(token[2]);
                    y2 = CSVImportHelper.ToNumber<int>(token[3]);
                    storeEdgeInTable(ref EdgeTable, y1 + 1, x1 + 1, y2 + 1, x2 + 1);//storage of edges in edge table.
                }
            }

            ScanlineFill(ref matrix, ref EdgeTable, ref ActiveEdgeTuple);
        }

        private void ScanlineFill(ref double[,] matrix, ref EdgeTableTuple[] EdgeTable, ref EdgeTableTuple ActiveEdgeTuple)
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
                    storeEdgeInTuple(ref ActiveEdgeTuple, EdgeTable[i].buckets[j].
                             ymax, (int)EdgeTable[i].buckets[j].xOfMin,
                            EdgeTable[i].buckets[j].slopeInverse);
                }

                // 2. Remove from AET those edges for
                // which y=ymax (not involved in next scan line)
                removeEdgeByYmax(ref ActiveEdgeTuple, i);

                //sort AET (remember: ET is presorted)
                insertionSort(ref ActiveEdgeTuple);

                //3. Fill lines on scan line y by using pairs of x-coords from AET
                j = 0;
                //FillFlag = false;
                coordCount = 0;
                x1 = 0;
                x2 = 0;
                ymax1 = 0;
                ymax2 = 0;
                while (j < ActiveEdgeTuple.countEdgeBucket)
                {
                    if (coordCount % 2 == 0)
                    {
                        x1 = (int)(ActiveEdgeTuple.buckets[j].xOfMin);
                        ymax1 = ActiveEdgeTuple.buckets[j].ymax;
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
                        x2 = (int)ActiveEdgeTuple.buckets[j].xOfMin;
                        ymax2 = ActiveEdgeTuple.buckets[j].ymax;

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
                                matrix[i, k] = 0;
                            }
                            //printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i);
                        }

                    }

                    j++;
                }


                // 5. For each nonvertical edge remaining in AET, update x for new y
                updatexbyslopeinv(ref ActiveEdgeTuple);
            }
        }

        private void updatexbyslopeinv(ref EdgeTableTuple Tup)
        {
            int i;

            for (i = 0; i < Tup.countEdgeBucket; i++)
            {
                (Tup.buckets[i]).xOfMin += (Tup.buckets[i]).slopeInverse;
            }
        }

        private void removeEdgeByYmax(ref EdgeTableTuple Tup, int yy)
        {
            int i, j;
            for (i = 0; i < Tup.countEdgeBucket; i++)
            {
                if (Tup.buckets[i].ymax == yy)
                {
                    for (j = i; j < Tup.countEdgeBucket - 1; j++)
                    {
                        Tup.buckets[j].ymax = Tup.buckets[j + 1].ymax;
                        Tup.buckets[j].xOfMin = Tup.buckets[j + 1].xOfMin;
                        Tup.buckets[j].slopeInverse = Tup.buckets[j + 1].slopeInverse;
                    }
                    Tup.countEdgeBucket--;
                    i--;
                }
            }
        }

        private void storeEdgeInTable(ref EdgeTableTuple[] EdgeTable, int x1, int y1, int x2, int y2)
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

            storeEdgeInTuple(ref EdgeTable[scanline], ymaxTS, xwithyminTS, minv);
        }

        private void storeEdgeInTuple(ref EdgeTableTuple receiver, int ym, int xm, double slopInv)
        {
            receiver.buckets[receiver.countEdgeBucket].ymax = ym;
            receiver.buckets[receiver.countEdgeBucket].xOfMin = (float)xm;
            receiver.buckets[receiver.countEdgeBucket].slopeInverse = slopInv;

            insertionSort(ref receiver);

            receiver.countEdgeBucket++;
        }

        private void insertionSort(ref EdgeTableTuple ett)
        {
            int i, j;
            EdgeBucket temp;

            for (i = 1; i < ett.countEdgeBucket; i++)
            {
                temp.ymax = ett.buckets[i].ymax;
                temp.xOfMin = ett.buckets[i].xOfMin;
                temp.slopeInverse = ett.buckets[i].slopeInverse;
                j = i - 1;

                while ((j >= 0) && (temp.xOfMin < ett.buckets[j].xOfMin))
                {
                    ett.buckets[j + 1].ymax = ett.buckets[j].ymax;
                    ett.buckets[j + 1].xOfMin = ett.buckets[j].xOfMin;
                    ett.buckets[j + 1].slopeInverse = ett.buckets[j].slopeInverse;
                    j--;
                }
                ett.buckets[j + 1].ymax = temp.ymax;
                ett.buckets[j + 1].xOfMin = temp.xOfMin;
                ett.buckets[j + 1].slopeInverse = temp.slopeInverse;
            }
        }

        private void initEdgeTable(ref EdgeTableTuple[] EdgeTable, ref EdgeTableTuple ActiveEdgeTuple)
        {
            for (int i = 0; i < M; i++)
            {
                EdgeTable[i].countEdgeBucket = 0;
            }

            ActiveEdgeTuple.countEdgeBucket = 0;
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
    sensors[key].lut;// = QList<DoubleMatrix>();
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
                                mat.init([sensors[key].n, sensors[key].m]);
            double[,] a = sensors[key].lut[intensity];
            for (int i = 0; i < sensors[key].n; i++)
            {
                for (int j = 0; j < sensors[key].m; j++)
                {
                    mat[i, j] = a[(int)(i * b), j];
                }
            }
            lut2.Add(mat);
        }
    }
    if (sensors[key].p.ratio > 1)
    {
        sensors[key].m = (int)(sensors[key].p.ratio * sensors[key].m);
        for (int intensity = 0; intensity < intLvls; intensity++)
        {
            double[,] mat = new double[sensors[key].n, sensors[key].m];
            double[,] a = sensors[key].lut[intensity];
            for (int i = 0; i < sensors[key].n; i++)
            {
                for (int j = 0; j < sensors[key].m; j++)
                {
                    mat[i, j] = a[(int)(i), (int)(j * sensors[key].p.ratio)];
                }
            }
            lut2.Add(mat);
        }
    }

    sensors[key].lut = lut2;

    //---------------------------------------
}

        private void generateMat(int key)
        {
            //if (!isInited) return;
            for (int intensity = 0; intensity < intLvls; intensity++)
            {
                double[,] mat = new double[sensors[key].n, sensors[key].m];
                for (int i = 0; i < sensors[key].n; i++)
                {
                    for (int j = 0; j < sensors[key].m; j++)
                    {
                        mat[i, j] = 0;
                    }
                }

                BivariateNormalPdf(ref mat, intensity + 1, sensors[key].n);

                sensors[key].lut.Add(mat);
            }
        }

        private void BivariateNormalPdf(ref double[,] matrix, int height, int n)
        {
            double[] mu = { 0, 0 };


            double[,] sigma = new double[2, 2] //SZÓRÁS
            {
                { n * szoras, 0 } ,
                { 0, n * szoras }
            };

            double[,] isigma = new double[2, 2];

            invMat(sigma, ref isigma, 2);
            double detSigma = determinantOfMatrix(sigma, 2);
            double coeff = 1 / (2 * Math.PI * Math.Sqrt(detSigma));

            double max = 0;

            //bool high = ratio < 1 ? true : false;
            //int skipCond = (int)(high ? 1 / ratio : ratio);

            for (int i = 0; i < n; i++)
            {
                double x = i - (n / 2.0);
                for (int j = 0; j < n; j++)
                {
                    double y = j - (n / 2.0);
                    double[] p = { x, y };
                    double[] pm = new double[2];
                    pm[0] = p[0] - mu[0];
                    pm[1] = p[1] - mu[1];
                    double a = (pm[0] * isigma[0, 0]) + (pm[1] * isigma[0, 1]);
                    double b = (pm[0] * isigma[1, 0]) + (pm[1] * isigma[1, 1]);
                    double c = p[0] * a;
                    double d = p[1] * b;
                    double v = coeff * Math.Exp(-0.5 * (c + d));
                    matrix[i, j] = v;
                    if (matrix[i, j] > max) max = matrix[i, j];
                }
            }

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    double v1 = matrix[i, j];
                    double v2 = height / max;
                    matrix[i, j] = (v1 * v2);
                }
            }
        }

        private double determinantOfMatrix(double[,] mat, int n)
        {
            int D = 0; // Initialize result

            //  Base case : if matrix contains single element
            if (n == 1)
                return mat[0, 0];

            double[,] temp = new double[n, n];

            int sign = 1;  // To store sign multiplier

            // Iterate for each element of first row
            for (int f = 0; f < n; f++)
            {
                // Getting Cofactor of mat[0][f]
                getCofactor(mat, ref temp, 0, f, n);
                D += (int)(sign * mat[0, f] * determinantOfMatrix(temp, n - 1));

                // terms are to be added with alternate sign
                sign = -sign;
            }

            return D;
        }

        private void getCofactor(double[,] mat, ref double[,] temp, int p, int q, int n)
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
                        double v = mat[row, col];
                        temp[i, j++] = v;

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

        private bool invMat(double[,] mat, ref double[,] inv, int a)
        {
            double det = determinantOfMatrix(mat, a);
            if (det == 0)
            {
                return false;
            }
            double[,] adj = new double[a, a];
            adjMat(mat, ref adj, a);
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < a; j++)
                {
                    double v = adj[i, j] / det;
                    inv[i, j] = v;
                }
            }
            return true;
        }

        private void adjMat(double[,] mat, ref double[,] adj, int a)
        {
            if (a == 1)
            {
                adj[0, 0] = 1; return;
            }
            int s;// = 1;
            double[,] t = new double[a, a];
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < a; j++)
                {
                    //To get cofactor of M[i][j]
                    getCofactor(mat, ref t, i, j, a);
                    s = ((i + j) % 2 == 0) ? 1 : -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
                    double v = s * (determinantOfMatrix(t, a - 1)); //Interchange rows and columns to get the transpose of the cofactor matrix
                    adj[j, i] = v;
                }
            }
        }



        public string filename;

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

