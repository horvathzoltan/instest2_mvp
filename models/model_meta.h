#define META_Model_InsoleType(m) Model::InsoleType r; \
Meta<Model::InsoleType> m(&r); \
m.AddRow(int,&r.Id); \
m.AddRow(QDateTime,&r.LastModified); \
m.AddRow(QString,&r.Name); \
m.AddRow(int,&r.InsoleGenderId); \
m.AddRow(int,&r.InsoleAgeCategoryId); \
m.AddRow(int,&r.InsoleSideId); \
m.AddRow(qreal,&r.EUSize); \
m.AddRow(QString,&r.GeometryCSV); \
m.AddRow(int,&r.R); \
m.AddRow(int,&r.VMax); \
m.AddRow(int,&r.VMin);
