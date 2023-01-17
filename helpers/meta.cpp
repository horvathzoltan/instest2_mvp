#include "meta.h"
#include "meta.h"
#include "meta.h"
#include "meta.h"

QString MetaHelper_GetRowName(const QString &n)
{
    int ix=n.indexOf('.');
    return ix>-1?n.mid(ix+1):QString();
}






