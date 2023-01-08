#ifndef MACRO_H
#define MACRO_H

#define STRINGIFY(msg) #msg
#define STRING(msg) QStringLiteral(STRINGIFY(msg))
#define L(msg) QStringLiteral(msg)

#endif // MACRO_H
