#define META_Model_InsoleType_Model(m) Model::InsoleType_Model r; \
Meta<Model::InsoleType_Model> m(&r); \ \
m.AddRow(static,&r.const); \
m.AddRow(static,&r.const); \
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
m.AddRow(int,&r.VMin); \
m.AddRow(static,&r.QList<InsoleType_Model>); \
m.AddRow(static,&r.InsoleType_Model); \
m.AddRow(static,&r.Meta<Model::InsoleType_Model>);