#include "model.h"
#include "logger.h"
#include "helpers/meta.h"


//QString Model::ApiVer::toString() const
//{
//    return QString::number(buildnum);
//}

const QString Model::InsoleType_Model::CSV = R"((1,'2020-02-08 14:15:00.0000000',N'42Jobb',1,2,3,42.0,N'insole;channel;x;y;r;ratio
42j16sd;RIGHT;-1;-1;none;none
42j16s;4;19;239;18.8;0.555556
42j16s;5;49;235;18.2857;0.555556
42j16s;2;18;178;27;0.340426
42j16s;3;38;173;22.3333;0.272727
42j16s;6;64;212;14.2857;0.62963
42j16s;7;55;168;19.6667;0.228571
42j16s;8;72;167;17.5;0.236842
42j16s;1;29;134;12.6667;0.777778
42j16s;9;61;121;18.3333;0.533333
42j16s;10;55;81;18.3333;0.5
42j16s;15;22;79;12;0.578947
42j16s;14;26;50;15.3333;0.695652
42j16s;11;49;47;15.3333;0.695652
42j16s;13;18;24;10;0.526316
42j16s;12;42;19;10.6;0.947368
42j16sf;none;25;275;none;none
42j16sf;none;25;275;none;none
42j16sf;none;35;278;none;none
42j16sf;none;44;278;none;none
42j16sf;none;52;275;none;none
42j16sf;none;59;270;none;none
42j16sf;none;66;261;none;none
42j16sf;none;80;232;none;none
42j16sf;none;85;214;none;none
42j16sf;none;87;191;none;none
42j16sf;none;85;167;none;none
42j16sf;none;80;139;none;none
42j16sf;none;73;104;none;none
42j16sf;none;69;71;none;none
42j16sf;none;64;44;none;none
42j16sf;none;62;26;none;none
42j16sf;none;57;15;none;none
42j16sf;none;50;7;none;none
42j16sf;none;45;2;none;none
42j16sf;none;35;0;none;none
42j16sf;none;26;0;none;none
42j16sf;none;22;1;none;none
42j16sf;none;15;5;none;none
42j16sf;none;10;8;none;none
42j16sf;none;6;16;none;none
42j16sf;none;3;24;none;none
42j16sf;none;2;41;none;none
42j16sf;none;8;85;none;none
42j16sf;none;10;125;none;none
42j16sf;none;0;193;none;none
42j16sf;none;0;210;none;none
42j16sf;none;2;242;none;none
42j16sf;none;7;256;none;none
42j16sf;none;25;275;none;none
42j16sw;4;1.2;-1;none;none
42j16sw;3;1;-1;none;none
42j16sw;7;1.1;-1;none;none
42j16sw;6;0.5;-1;none;none
42j16sw;2;1.4;-1;none;none
42j16sw;5;1.2;-1;none;none
42j16sw;1;0.8;-1;none;none
42j16sw;11;0.6;-1;none;none
42j16sw;15;1.1;-1;none;none
42j16sw;14;0.5;-1;none;none
42j16sw;10;1;-1;none;none
42j16sw;9;0.9;-1;none;none
42j16sw;13;0.6;-1;none;none
42j16sw;8;1.4;-1;none;none
42j16sw;12;0.8;-1;none;none',390,53,46),
     (2,'2020-02-08 14:20:00.0000000',N'42Bal',1,2,4,42.0,N'insole;channel;x;y;r;ratio
42b16sd;LEFT;-1;-1;none;none
42b16s;12;68;239;18.8;0.555556
42b16s;11;38;235;18.2857;0.555556
42b16s;14;69;178;27;0.340426
42b16s;13;48;173;22.3333;0.272727
42b16s;10;23;212;14.2857;0.62963
42b16s;9;32;168;19.6667;0.228571
42b16s;8;15;167;17.5;0.236842
42b16s;15;58;134;12.6667;0.777778
42b16s;7;25;121;18.3333;0.533333
42b16s;6;32;81;18.3333;0.5
42b16s;1;64;79;12;0.578947
42b16s;2;61;50;15.3333;0.695652
42b16s;5;37;47;15.3333;0.695652
42b16s;3;69;24;10;0.526316
42b16s;4;44;19;10.6;0.947368
42b16sf;none;62;275;none;none
42b16sf;none;62;275;none;none
42b16sf;none;52;278;none;none
42b16sf;none;43;278;none;none
42b16sf;none;35;275;none;none
42b16sf;none;27;270;none;none
42b16sf;none;21;261;none;none
42b16sf;none;7;232;none;none
42b16sf;none;2;214;none;none
42b16sf;none;0;191;none;none
42b16sf;none;2;167;none;none
42b16sf;none;7;139;none;none
42b16sf;none;13;104;none;none
42b16sf;none;17;71;none;none
42b16sf;none;22;44;none;none
42b16sf;none;25;26;none;none
42b16sf;none;30;15;none;none
42b16sf;none;36;7;none;none
42b16sf;none;41;2;none;none
42b16sf;none;52;0;none;none
42b16sf;none;60;0;none;none
42b16sf;none;64;1;none;none
42b16sf;none;72;5;none;none
42b16sf;none;77;8;none;none
42b16sf;none;81;16;none;none
42b16sf;none;83;24;none;none
42b16sf;none;85;41;none;none
42b16sf;none;78;85;none;none
42b16sf;none;77;125;none;none
42b16sf;none;87;193;none;none
42b16sf;none;87;210;none;none
42b16sf;none;85;242;none;none
42b16sf;none;80;256;none;none
42b16sf;none;62;275;none;none
42b16sw;12;1.2;-1;none;none
42b16sw;13;1;-1;none;none
42b16sw;9;1.1;-1;none;none
42b16sw;10;0.5;-1;none;none
42b16sw;14;1.4;-1;none;none
42b16sw;11;1.2;-1;none;none
42b16sw;15;0.8;-1;none;none
42b16sw;5;0.6;-1;none;none
42b16sw;1;1.1;-1;none;none
42b16sw;2;0.5;-1;none;none
42b16sw;6;1;-1;none;none
42b16sw;7;0.9;-1;none;none
42b16sw;3;0.6;-1;none;none
42b16sw;8;1.4;-1;none;none
42b16sw;4;0.8;-1;none;none',470,63,54),
     (3,'2020-02-08 14:20:00.0000000',N'36Jobb',1,2,3,36.0,N'insole;channel;x;y;r;ratio
36j16sd;RIGHT;-1;-1;none;none
36j16s;4;24;210;13.4;0.775
36j16s;5;42;210;9.2;0.439024
36j16s;6;54;197;11.6667;0.574468
36j16s;2;13;165;19.6;0.1625
36j16s;3;32;168;24.25;0.227848
36j16s;7;49;154;18;0.285714
36j16s;8;63;155;16.1667;0.257143
36j16s;1;21;121;12;0.540541
36j16s;9;53;109;18.3333;0.419355
36j16s;10;42;68;17.3333;0.316667
36j16s;15;19;65;11.3333;0.421053
36j16s;14;21;43;10;0.758621
36j16s;11;42;41;10.3333;0.862069
36j16s;13;16;21;8.6;0.486486
36j16s;12;38;20;10.25;0.828571
36j16sf;none;1;152;none;none
36j16sf;none;6;126;none;none
36j16sf;none;7;100;none;none
36j16sf;none;6;72;none;none
36j16sf;none;5;59;none;none
36j16sf;none;2;37;none;none
36j16sf;none;1;26;none;none
36j16sf;none;5;13;none;none
36j16sf;none;10;5;none;none
36j16sf;none;17;1;none;none
36j16sf;none;24;0;none;none
36j16sf;none;30;0;none;none
36j16sf;none;36;1;none;none
36j16sf;none;41;3;none;none
36j16sf;none;48;10;none;none
36j16sf;none;51;14;none;none
36j16sf;none;54;23;none;none
36j16sf;none;57;36;none;none
36j16sf;none;60;62;none;none
36j16sf;none;62;78;none;none
36j16sf;none;66;99;none;none
36j16sf;none;71;126;none;none
36j16sf;none;73;138;none;none
36j16sf;none;76;156;none;none
36j16sf;none;76;172;none;none
36j16sf;none;73;191;none;none
36j16sf;none;67;212;none;none
36j16sf;none;62;219;none;none
36j16sf;none;59;224;none;none
36j16sf;none;52;231;none;none
36j16sf;none;44;235;none;none
36j16sf;none;31;237;none;none
36j16sf;none;25;234;none;none
36j16sf;none;19;230;none;none
36j16sf;none;12;220;none;none
36j16sf;none;5;208;none;none
36j16sf;none;0;191;none;none
36j16sf;none;0;172;none;none
36j16sf;none;1;152;none;none
36j16sw;4;1,4;-1;none;none
36j16sw;5;0,1;-1;none;none
36j16sw;6;1;-1;none;none
36j16sw;2;1,7;-1;none;none
36j16sw;3;1,3;-1;none;none
36j16sw;7;1,4;-1;none;none
36j16sw;8;1,4;-1;none;none
36j16sw;1;1,7;-1;none;none
36j16sw;9;1,3;-1;none;none
36j16sw;10;1,4;-1;none;none
36j16sw;15;1,5;-1;none;none
36j16sw;14;0,5;-1;none;none
36j16sw;11;0,6;-1;none;none
36j16sw;13;0,6;-1;none;none
36j16sw;12;0,8;-1;none;none',560,73,64),
     (4,'2020-02-08 14:20:00.0000000',N'36Bal',1,2,4,36.0,N'insole;channel;x;y;r;ratio
36b16sd;LEFT;-1;-1;none;none
36b16s;12;52;210;13.4;0.775
36b16s;11;34;210;9.2;0.439024
36b16s;10;22;197;11.6667;0.574468
36b16s;14;62;165;19.6;0.1625
36b16s;13;44;168;24.25;0.227848
36b16s;9;27;154;18;0.285714
36b16s;8;13;155;16.1667;0.257143
36b16s;15;55;121;12;0.540541
36b16s;7;23;109;18.3333;0.419355
36b16s;6;33;68;17.3333;0.316667
36b16s;1;56;65;11.3333;0.421053
36b16s;2;55;43;10;0.758621
36b16s;5;33;41;10.3333;0.862069
36b16s;3;60;21;8.6;0.486486
36b16s;4;38;20;10.25;0.828571
36b16sf;none;75;152;none;none
36b16sf;none;69;126;none;none
36b16sf;none;69;100;none;none
36b16sf;none;70;72;none;none
36b16sf;none;71;59;none;none
36b16sf;none;74;37;none;none
36b16sf;none;74;26;none;none
36b16sf;none;71;13;none;none
36b16sf;none;66;5;none;none
36b16sf;none;59;1;none;none
36b16sf;none;52;0;none;none
36b16sf;none;46;0;none;none
36b16sf;none;40;1;none;none
36b16sf;none;35;3;none;none
36b16sf;none;28;10;none;none
36b16sf;none;25;14;none;none
36b16sf;none;22;23;none;none
36b16sf;none;19;36;none;none
36b16sf;none;15;62;none;none
36b16sf;none;13;78;none;none
36b16sf;none;10;99;none;none
36b16sf;none;5;126;none;none
36b16sf;none;3;138;none;none
36b16sf;none;0;156;none;none
36b16sf;none;0;172;none;none
36b16sf;none;3;191;none;none
36b16sf;none;9;212;none;none
36b16sf;none;13;219;none;none
36b16sf;none;17;224;none;none
36b16sf;none;24;231;none;none
36b16sf;none;32;235;none;none
36b16sf;none;45;237;none;none
36b16sf;none;51;234;none;none
36b16sf;none;57;230;none;none
36b16sf;none;64;220;none;none
36b16sf;none;71;208;none;none
36b16sf;none;76;191;none;none
36b16sf;none;76;172;none;none
36b16sf;none;75;152;none;none
36b16sw;12;1,4;-1;none;none
36b16sw;11;0,1;-1;none;none
36b16sw;10;1;-1;none;none
36b16sw;14;1,7;-1;none;none
36b16sw;13;1,3;-1;none;none
36b16sw;9;1,4;-1;none;none
36b16sw;8;1,4;-1;none;none
36b16sw;15;1,7;-1;none;none
36b16sw;7;1,3;-1;none;none
36b16sw;6;1,4;-1;none;none
36b16sw;1;1,5;-1;none;none
36b16sw;2;0,5;-1;none;none
36b16sw;5;0,6;-1;none;none
36b16sw;3;0,6;-1;none;none
36b16sw;4;0,8;-1;none;none',680,87,76),
     (5,'2020-02-08 14:20:00.0000000',N'39Jobb',1,2,3,39.0,N'insole;channel;x;y;r;ratio
39j16sd;RIGHT;-1;-1;none;none
39j16s;4;23;233;13.8333;0.659574
39j16s;5;43;231;15;0.471698
39j16s;6;61;215;13.25;0.617021
39j16s;2;12;179;22;0.191489
39j16s;3;32;173;23;0.25
39j16s;7;52;171;22.6667;0.265823
39j16s;8;67;170;19.6;0.258824
39j16s;1;22;134;11.6667;0.567568
39j16s;9;57;121;18.3333;0.5
39j16s;10;44;81;17.3333;0.316667
39j16s;15;20;78;11.3333;0.486486
39j16s;14;22;49;14.6667;0.630435
39j16s;11;44;47;14.6667;0.608696
39j16s;13;16;21;9.2;0.5
39j16s;12;39;21;10.2;0.789474
39j16sf;none;38;265;none;none
39j16sf;none;38;265;none;none
39j16sf;none;49;262;none;none
39j16sf;none;58;255;none;none
39j16sf;none;66;245;none;none
39j16sf;none;72;233;none;none
39j16sf;none;77;217;none;none
39j16sf;none;81;200;none;none
39j16sf;none;83;172;none;none
39j16sf;none;80;153;none;none
39j16sf;none;76;141;none;none
39j16sf;none;67;101;none;none
39j16sf;none;64;85;none;none
39j16sf;none;59;37;none;none
39j16sf;none;55;22;none;none
39j16sf;none;48;10;none;none
39j16sf;none;42;5;none;none
39j16sf;none;32;0;none;none
39j16sf;none;17;1;none;none
39j16sf;none;7;8;none;none
39j16sf;none;2;18;none;none
39j16sf;none;0;30;none;none
39j16sf;none;1;50;none;none
39j16sf;none;7;85;none;none
39j16sf;none;6;139;none;none
39j16sf;none;0;170;none;none
39j16sf;none;0;193;none;none
39j16sf;none;0;214;none;none
39j16sf;none;5;232;none;none
39j16sf;none;10;247;none;none
39j16sf;none;20;257;none;none
39j16sf;none;38;265;none;none
39j16sw;4;1,2;-1;none;none
39j16sw;5;0,7;-1;none;none
39j16sw;6;0,8;-1;none;none
39j16sw;2;1,6;-1;none;none
39j16sw;3;1,5;-1;none;none
39j16sw;7;1,2;-1;none;none
39j16sw;8;1,2;-1;none;none
39j16sw;1;0,8;-1;none;none
39j16sw;9;1,1;-1;none;none
39j16sw;10;1,1;-1;none;none
39j16sw;15;1,2;-1;none;none
39j16sw;14;0,6;-1;none;none
39j16sw;11;0,6;-1;none;none
39j16sw;13;0,9;-1;none;none
39j16sw;12;0,8;-1;none;none',820,103,89),
     (6,'2020-02-08 14:20:00.0000000',N'39Bal',1,2,4,39.0,N'insole;channel;x;y;r;ratio
39b16sd;LEFT;-1;-1;none;none
39b16s;12;60;233;13.8333;0.659574
39b16s;11;40;231;15;0.471698
39b16s;10;21;215;13.25;0.617021
39b16s;14;70;179;22;0.191489
39b16s;13;50;173;23;0.25
39b16s;9;30;171;22.6667;0.265823
39b16s;8;15;170;19.6;0.258824
39b16s;15;60;134;11.6667;0.567568
39b16s;7;25;121;18.3333;0.5
39b16s;6;38;81;17.3333;0.316667
39b16s;1;62;78;11.3333;0.486486
39b16s;2;61;49;14.6667;0.630435
39b16s;5;38;47;14.6667;0.608696
39b16s;3;67;21;9.2;0.5
39b16s;4;43;21;10.2;0.789474
39b16sf;none;44;265;none;none
39b16sf;none;44;265;none;none
39b16sf;none;33;262;none;none
39b16sf;none;24;255;none;none
39b16sf;none;16;245;none;none
39b16sf;none;10;233;none;none
39b16sf;none;5;217;none;none
39b16sf;none;1;200;none;none
39b16sf;none;0;172;none;none
39b16sf;none;2;153;none;none
39b16sf;none;6;141;none;none
39b16sf;none;15;101;none;none
39b16sf;none;18;85;none;none
39b16sf;none;24;37;none;none
39b16sf;none;27;22;none;none
39b16sf;none;34;10;none;none
39b16sf;none;40;5;none;none
39b16sf;none;50;0;none;none
39b16sf;none;65;1;none;none
39b16sf;none;75;8;none;none
39b16sf;none;80;18;none;none
39b16sf;none;83;30;none;none
39b16sf;none;81;50;none;none
39b16sf;none;75;85;none;none
39b16sf;none;76;139;none;none
39b16sf;none;83;170;none;none
39b16sf;none;83;193;none;none
39b16sf;none;83;214;none;none
39b16sf;none;78;232;none;none
39b16sf;none;72;247;none;none
39b16sf;none;62;257;none;none
39b16sf;none;44;265;none;none
39b16sw;12;1,2;-1;none;none
39b16sw;11;0,7;-1;none;none
39b16sw;10;0,8;-1;none;none
39b16sw;14;1,6;-1;none;none
39b16sw;13;1,5;-1;none;none
39b16sw;9;1,2;-1;none;none
39b16sw;8;1,2;-1;none;none
39b16sw;15;0,8;-1;none;none
39b16sw;7;1,1;-1;none;none
39b16sw;6;1,1;-1;none;none
39b16sw;1;1,2;-1;none;none
39b16sw;2;0,6;-1;none;none
39b16sw;5;0,6;-1;none;none
39b16sw;3;0,9;-1;none;none
39b16sw;4;0,8;-1;none;none',1000,121,106)";

const QString Model::InsoleType_Model::CSV_header =
        R"(Id,LastModified,Name,InsoleGenderId,InsoleAgeCategoryId,InsoleSideId,EUSize,GeometryCSV,R,VMax,VMin)";

QList<Model::InsoleType_Model> Model::InsoleType_Model::ParseList(const QString &str)
{
    QStringList tokens = str.split(",\n");

    QList<Model::InsoleType_Model> r;
    for(auto&token:tokens){
        int ix1 = token.indexOf(QChar('('));
        int ix2 = token.lastIndexOf(QChar(')'));
        int ix= ix1+1;
        int length = ix2-ix1-1;
        token = token.mid(ix,length);
        //zInfo("token:"+token);
        InsoleType_Model insoleType = Parse(token);
        r.append(insoleType);
    }
    return r;
}


Model::InsoleType_Model Model::InsoleType_Model::Parse(const QString &str)
{    
    static QStringList h_tokens = CSV_header.split(",");
      static auto meta = GetMeta();
      Model::InsoleType_Model r;
      meta.base = &r;

      QString g("2020-02-08 14:15:00.0000000");
      QDateTime t = QDateTime::fromString(g, "yyyy-MM-dd hh:mm:ss.zzz0000");

      QStringList tokens;
      bool in=false;
      int ix0 = 0;
      int L = str.length();
      int MAX_IX = L-1;
      for(int i=0;i<L;i++){
          auto& a= str[i];
          if(a=='\'') in=!in;
          else if(!in)
          {
              if(i==MAX_IX) {
                  tokens.append(str.mid(ix0));
              } else if(a==','){
                  tokens.append(str.mid(ix0,i-ix0));
                  ix0=i+1;
              }
          }
      }

      if(tokens.length()<11) return r;
      if(tokens.length()<h_tokens.length()) return r;
      for(int i=0;i<h_tokens.length();i++){
          QString name = h_tokens[i];
          QString value = tokens[i];

          // nvarchar esetén
          if(value.startsWith("N'") && value.endsWith('\''))
          {
              value = value.mid(2, value.length()-3);
          }
          // datetime esetén
          else if(value.startsWith("'") && value.endsWith('\''))
          {
              value = value.mid(1, value.length()-2);
          }
          meta.Parse(name, value);
      }
      return r;
}

#define META_Model_InsoleType(m) Model::InsoleType_Model r; \
Meta<Model::InsoleType_Model> m(&r); \
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

Meta<Model::InsoleType_Model> Model::InsoleType_Model::GetMeta()
{

#ifdef META_Model_InsoleType
    META_Model_InsoleType(model)
    return model;
#else
    zInfo("META_Model_InsoleType not defined");
#endif
}

/*
//Model::InsoleType r;
//Meta<Model::InsoleType> meta(&r);
//    Model::InsoleType r;
//    Meta<Model::InsoleType> meta(&r);
//    meta.AddRow(int,&r.Id); //0
//    meta.AddRow(QDateTime,&r.LastModified); //1
//    meta.AddRow(QString,&r.Name); //2
//    meta.AddRow(int,&r.InsoleGenderId); //3
//    meta.AddRow(int,&r.InsoleAgeCategoryId); //4
//    meta.AddRow(int,&r.InsoleSideId); //5
//    meta.AddRow(qreal,&r.EUSize); //6
//    meta.AddRow(QString,&r.GeometryCSV); //7
//    meta.AddRow(int,&r.R); //8
//    meta.AddRow(int,&r.VMax); //9
//    meta.AddRow(int,&r.VMin); //10
*/

/*cut here*/

